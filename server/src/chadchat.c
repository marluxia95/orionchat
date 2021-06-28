#include "chadchat.h"

#define MAX_BUFSIZE 2048
#define MAX_CLIENTS 12

bool active = true;
int cli_count = 0;

static struct {
	struct sockaddr_in s_address;
	int sfd;
} server;

client_t* clients[MAX_CLIENTS];

pthread_mutex_t clients_lock;

void clear_buffer(char* buf)
{
	memset(buf, 0, sizeof(buf));
}

/* Adds a new client */
void add_client(client_t* cli)
{
	pthread_mutex_lock(&clients_lock);
	for(int ci = 0; ci < MAX_CLIENTS; ci++){
		if(!clients[ci])
			clients[ci] = cli;
	}
	pthread_mutex_unlock(&clients_lock);
}

/* Removes a client */
void remove_client(int id)
{
	pthread_mutex_lock(&clients_lock);
	for(int ci = 0; ci < MAX_CLIENTS; ci++){
		if(clients[ci]->id == id)
			clients[ci] = NULL;
	}
	pthread_mutex_unlock(&clients_lock);
}

/* Close all server connections */
void close_all()
{

}

/* Sends a message to a client */
void send_to(const char* msg, int cfd)
{
	if(write(cfd, msg, strlen(msg)) < 0){
		perror("Failed to send message");
	}
}


/* Sends a message to all clients BUT the sender */
void send_all(const char* msg, int senderid)
{
	pthread_mutex_lock(&clients_lock);
	for(int ci = 0; ci < MAX_CLIENTS; ci++){
		if(clients[ci]){
			if(clients[ci]->id != senderid)
				write(clients[ci]->cfd, msg, sizeof(msg));
		}
	}
	pthread_mutex_unlock(&clients_lock);
}

/* Sends a message to all clients */
void broadcast(const char* msg)
{
	pthread_mutex_lock(&clients_lock);
	for(int ci = 0; ci < MAX_CLIENTS; ci++){
		if(clients[ci])
			write(clients[ci]->cfd, msg, sizeof(msg));
	}
	pthread_mutex_unlock(&clients_lock);
}

/* Handles a client connection ( always run in a thread ) */
void* client_handler(void* c_arg)
{
	cli_count++;

	client_t* cli = (client_t*)c_arg;

	char address_string[INET_ADDRSTRLEN] = {0};
	inet_ntop(AF_INET, &(cli->c_address.sin_addr), address_string, INET_ADDRSTRLEN);

	printf("Client %s ( %d ) connected\n", address_string, cli->cfd);
		
	char input_buffer[MAX_BUFSIZE];
	char output_buffer[MAX_BUFSIZE];
	int read_length;

	puts("Sending msg");

	send_to("Welcome!", cli->cfd);

	sprintf(output_buffer, "%d Joined the chat!", cli->id);
	send_all(output_buffer, cli->id);

		puts("Leave");
		sprintf(output_buffer, "%d left the server", cli->id);
		send_all(output_buffer, cli->id);
		close(cli->cfd);
		remove_client(cli->id);
		pthread_exit(NULL);


	pthread_exit(NULL);
}

void server_init(const char* address, int port)
{
	server.sfd = 0;

	server.sfd = socket(AF_INET, SOCK_STREAM, 0);
	server.s_address.sin_family = AF_INET;
	server.s_address.sin_port = htons(port);

	atexit(server_close);

	if(!inet_pton(AF_INET, address, &server.s_address.sin_addr)){
		puts("Invalid server address");
		exit(EXIT_FAILURE);
	}

	if(bind(server.sfd, (struct sockaddr*)&server.s_address, sizeof(server.s_address)) < 0){
		perror("Bind failed");
		exit(EXIT_FAILURE);
	}

	if(listen(server.sfd, 3) < 0){
		perror("Listen");
		exit(EXIT_FAILURE);
	}
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

		// Add connection handler shits
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
	if(argc < 3){
		printf("Simple socket server \n     Usage : %s [host] [port] \n", argv[0]);
		return 0;		
	}

	int port = atoi(argv[2]);	
	

	
	

	return 0;

}
