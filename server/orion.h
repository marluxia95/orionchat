#ifndef ORION_H
#define ORION_H

#define MAX_BUFSIZE 1500
#define MAX_CLIENTS 40

#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

typedef struct {
	struct sockaddr_in c_address;
	int cfd;
	int id;
	char name[16];
} client_t;

static pthread_mutex_t clients_lock;

static client_t* clients[MAX_CLIENTS];

#endif