#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdbool.h>
#include <pthread.h>

#include "orion_core.h"
#include "orion_prot.h"
#include "orion_array.h"
#include "orion.h"

bool active = true;
int cli_count = 0;

static struct {
	struct sockaddr_in s_address;
	int sfd;
} server;

void clear_buffer(char* buf)
{
	memset(buf, 0, sizeof(buf));
}

// Adds a new client 
void add_client(client_t* cli)
{
	pthread_mutex_lock(&clients_lock);
	for(int ci = 0; ci < MAX_CLIENTS; ci++){
		if(!clients[ci]){
			puts("Added client");
			clients[ci] = cli;
			break;
		}
	}
	pthread_mutex_unlock(&clients_lock);
}

void remove_client(int id)
{
	pthread_mutex_lock(&clients_lock);
	printf("Looking for client with id %d \n", id);
	for(int ci = 0; ci < MAX_CLIENTS; ci++){
		if(clients[ci] != NULL){
			if(clients[ci]->id == id) {
				printf("Checking %d with id %d\n", ci, clients[ci]->id);
				free(clients[ci]);
			}
		}
	}
	pthread_mutex_unlock(&clients_lock);
}

// Gets client's id using a socket file descriptor
int get_client_id(int cfd)
{
	pthread_mutex_lock(&clients_lock);
	for(int ci = 0; ci < MAX_CLIENTS; ci++){
		if(clients[ci]->cfd == cfd)
			return clients[ci]->id;
	}
	pthread_mutex_unlock(&clients_lock);
}

// Close all server connections 
void close_all()
{

}

// Handles a client connection ( always run in a thread ) 
void* client_handler(void* c_arg)
{
	cli_count++;
	int msglen;

	client_t* cli = (client_t*)c_arg;

	char address_string[INET_ADDRSTRLEN] = {0};
	inet_ntop(AF_INET, &(cli->c_address.sin_addr), address_string, INET_ADDRSTRLEN);

	printf("Client %s ( %d ) connected\n", address_string, cli->cfd);
		
	unsigned char input_buffer[MAX_BUFSIZE];
	unsigned char output_buffer[MAX_BUFSIZE];

	puts("Sending msg");

	ssend_to("Welcome!", cli->cfd);

	

	char** args;

	while((msglen = read(cli->cfd, input_buffer, MAX_BUFSIZE)) != 0 ){
		args = orion_dec(input_buffer, msglen);
		
		if(input_buffer[1] == C_NICK){
			printf("%s(%d) -> %s\n",address_string, cli->cfd , args[0]);
			strncpy(cli->name, args[0], 16);
			cli->status = STATUS_READY; // Client is ready to send messages
			snprintf(output_buffer, MAX_BUFSIZE, "%s Joined the chat!\n", cli->name);
			send_all(output_buffer, cli->id);
		}

		if(cli->status<STATUS_READY) // Check if client has set a nickname
			continue;

		if(input_buffer[1] == C_MSG){
			int argsize = (int)(((unsigned)input_buffer[2] << 8) | input_buffer[3]);
			if(argsize <= 255){ 
				printf("<%s> %s\n", cli->name, args[0]);
				send_all(args[0], cli->id);
			}
		}
		orion_clear(args);
		
	}
	
	puts("Leave");
	

	cli_count--;
	if(cli_count){
		if(cli->name)
			sprintf(output_buffer, "%s left the server", cli->name);
		else
			sprintf(output_buffer, "%d left the server", cli->id);

		puts(output_buffer);
		send_all(output_buffer, cli->id);
	}

	remove_client(cli->id);
	pthread_exit(NULL);
}

void server_loop()
{
	struct sockaddr_in cl_addr;

	pthread_t tid;
	
	int cfd = 0;
	int cid = 0;

	do {
		socklen_t cl_addr_len = sizeof(cl_addr);
		cfd = accept(server.sfd, (struct sockaddr*)&cl_addr, &cl_addr_len);
		cid++;
		
		client_t* new_client = (client_t*)malloc(sizeof(client_t));
		new_client->cfd = cfd;
		new_client->c_address = cl_addr;
		new_client->id = cid;

		add_client(new_client);

		pthread_create(&tid, NULL, &client_handler, (void*)new_client);
	} while (true);
}

void server_close()
{
	close(server.sfd);
	puts("Exiting");
}


int main (int argc, const char* argv[])
{
	char* address;
	int port;
	if(argc < 3){
		address = "0.0.0.0";
		port = 8080;
	}else{
		address = argv[1];
		port = atoi(argv[2]);	
	}

	server.sfd = 0;

	server.sfd = socket(AF_INET, SOCK_STREAM, 0);
	server.s_address.sin_family = AF_INET;
	server.s_address.sin_port = htons(port);


	if(!inet_pton(AF_INET, address, &server.s_address.sin_addr)){
		puts("Invalid server address");
		exit(EXIT_FAILURE);
	}

	printf("Server listening on %s:%d\n", address, port);

	if(bind(server.sfd, (struct sockaddr*)&server.s_address, sizeof(server.s_address)) < 0){
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

	if(listen(server.sfd, 3) < 0){
		perror("Listen");
		exit(EXIT_FAILURE);
	}

	server_loop();

	return 0;

}
