#ifndef ORION_CORE_H
#define ORION_CORE_H

#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <string.h>
#include <stdbool.h>
#include "orion.h"


void send_raw(void* data, size_t dsize, int cfd);
void send_all_raw(void* data, size_t dsize, int senderid);
void broadcast_raw(void* data, size_t dsize);

#endif