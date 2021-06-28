#ifndef __CORE_INCLUDE_
#define __CORE_INCLUDE_

#include "orion.h"

typedef struct {
	struct sockaddr_in c_address;
	int cfd;
	int id;
	char name[16];
} client_t;

void clear_buffer(char* buf);
void add_client(client_t* cli);
void remove_client(int id);

void close_all();

void send_to(const char* msg, int cfd);
void send_all(const char* msg, int senderid);
void broadcast(const char* msg);

#endif