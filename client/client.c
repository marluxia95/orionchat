#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include <string.h>

#include "orion_core.h"
#include "orion_prot.h"

#define PORT 8080
#define MAX_BUFSIZE 1024
#define MAX_MSGSIZE 255

int sock = 0;
struct sockaddr_in server_addr;
bool active = true;
bool readyToSend = false;
unsigned char input_buffer[MAX_BUFSIZE];

// Sends the client's nickname to server
void setNickname(const char* nickname)
{
	char* args[][16] = {nickname};
	orion_array_t* data_arr = orion_enc(C_NICK, 1, args);
	send_raw(data_arr->data, data_arr->used, sock);
	array_debug_print(data_arr);
	array_free(data_arr);
}

// Response handler thread
void* response_handler(void* arg)
{
	int datalen;
	while((datalen = read( sock, input_buffer, MAX_BUFSIZE )) != 0){
		if(!active){
			pthread_exit(0);
		}

		printf("%s\n", input_buffer);

		char** arguments = orion_dec(input_buffer, datalen);

		switch(input_buffer[1]){
			case C_MSG:
				printf("<%s> %s\n", arguments[0], arguments[1]);
				break;
			case C_RMSG:
				printf("%s\n", arguments[0]);
				break;
			case ERR_WRONG_NICK:
				printf("\e[1;31m Error : Invalid nickname \e[0m");
				break;
		}
		memset(&input_buffer, 0, MAX_BUFSIZE);
		printf("test %d\n", sizeof(arguments));
		orion_clear(arguments);
	}
	active = false;
	puts("Connection lost");
	pthread_exit(0);
}

char* input_getLine()
{
	char* buffer = malloc(MAX_BUFSIZE + 10);
	memset(buffer, 0, MAX_BUFSIZE + 10);
	char c;
	int pos = 0;

	while (true) {
		c = getchar();

		if(c == '\n' || c == EOF) {
			buffer[pos+1] = '\0';
			return buffer;
        }
		if(pos >= MAX_MSGSIZE)
			continue;
		
		buffer[pos] = c;
		pos++;
	}

}

void proccessInput(const char* msg)
{
	if(!strcmp(msg, "!quit")) {
		active = false;
	}else{
		send_to(msg, sock);
	}
}

int main ( int argc, char const* argv[] ) 
{
	

	if((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0){
		puts("Socket error");
		return -1;
	}

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(PORT);

	if(inet_pton(AF_INET, "127.0.0.1", &server_addr.sin_addr) <= 0){
		puts("Invalid address");
		return -1;
	}

	if (connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0 ){
		puts("Connection failed");
		return -1;
	}

	// Setup threads

	pthread_attr_t attr;
	pthread_t tid;
	pthread_attr_init (&attr);
	pthread_attr_setdetachstate (&attr, PTHREAD_CREATE_DETACHED);

	pthread_create(&tid, &attr, &response_handler, NULL);

	pthread_attr_destroy (&attr);

	char nickname[16];
	printf("Enter a nickname:");
	fgets(nickname, 16, stdin);
	nickname[strcspn(nickname, "\n")] = 0;
	
	setNickname(nickname);

	while(active) {
		char* msg = input_getLine();
		proccessInput(msg);
		free(msg);
	}

	printf("Goodbye fellow user!\n");

	pthread_detach(tid);

}
