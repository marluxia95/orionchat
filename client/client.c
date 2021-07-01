#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include "orion_core.h"

#define PORT 8080
#define MAX_BUFSIZE 1024

int sock = 0, valread;
struct sockaddr_in server_addr;
bool active = true;
char input_buffer[MAX_BUFSIZE];
char output_buffer[MAX_BUFSIZE];
int bufpos = 0;
char c;

void* response_handler(void* arg)
{
	while((valread = read( sock, input_buffer, MAX_BUFSIZE )) != 0){
		
		puts("Recv");
		printf("%s\n", input_buffer);

		memset(&input_buffer, 0, MAX_BUFSIZE);
	}
}

int main ( int argc, char const* argv[] ) 
{
	pthread_t tid;

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

	pthread_create(&tid, NULL, &response_handler, NULL);

	do{
		c = getchar();
		if(c == '\n' || c == EOF) {
            output_buffer[bufpos + 1] = '\0';
			send_to(output_buffer, sock);
			memset(&output_buffer, 0, bufpos);
			printf("Sent %d bytes", bufpos);
			bufpos = 0;
        }
        bufpos++;

	} while( active );
}
