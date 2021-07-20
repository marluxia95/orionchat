#ifndef ORION_H
#define ORION_H

#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>

#define MAX_BUFSIZE 1500
#define MAX_CLIENTS 40

#define ORION_DEBUG

enum conn_status {
	STATUS_NICKNAME,
	STATUS_READY
};

typedef struct {
	struct sockaddr_in c_address;
	int cfd;
	int id;
	int status;
	char name[16];
} client_t;

static pthread_mutex_t clients_lock;

static client_t* clients[MAX_CLIENTS];

#endif