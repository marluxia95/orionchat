#ifndef __CLIENT_INCLUDE_
#define __CLIENT_INCLUDE_

#include "chadchat.h"

typedef struct {
	struct sockaddr_in c_address;
	int cfd;
	int id;
	char name[16];
} client_t;

#endif