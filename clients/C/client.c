#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 8080

int main ( int argc, char const* argv[] ) 
{
	int sock = 0, valread;
	struct sockaddr_in server_addr;
	char buffer[2048] = {0};
	char* msg = "Hello World!";

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

	/*

	if(send(sock, msg, strlen(msg), 0) <0 ){
		puts("Failed to send message");
		return -1;
	}

	puts("Message sent");
	valread = read( sock, buffer, 2048 );
	printf("%s\n", buffer);


	
	*/
	
}
