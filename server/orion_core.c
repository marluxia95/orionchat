#include "orion_core.h"

// Sends a raw message to a client
void send_raw(void* data, size_t dsize, int cfd)
{
	if(write(cfd, data, dsize) < 0){
		perror("Failed to send message");
	}
}

// Sends a raw message to all clients BUT the sender
void send_all_raw(void* data, size_t dsize, int senderid)
{
	pthread_mutex_lock(&clients_lock);
	for(int ci = 0; ci < MAX_CLIENTS; ci++){
		if(clients[ci]){
			if(clients[ci]->id != senderid)
				write(clients[ci]->cfd, data, dsize);
		}
	}
	pthread_mutex_unlock(&clients_lock);
}

// Sends a raw message to all clients
void broadcast_raw(void* data, size_t dsize)
{
	pthread_mutex_lock(&clients_lock);
	for(int ci = 0; ci < MAX_CLIENTS; ci++){
		if(clients[ci])
			write(clients[ci]->cfd, data, dsize);
	}
	pthread_mutex_unlock(&clients_lock);
}
