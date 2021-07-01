#ifndef __ROOM_INCLUDE_
#define __ROOM_INCLUDE_

#include <stdio.h>
#include <pthread.h>
#include "orion.h"

#define MAX_CLIENTS_PER_ROOM 16384
#define MAX_CHATROOMS 255

typedef struct {
    client_t* clients[MAX_CLIENTS_PER_ROOM];
    unsigned int id;
} chatroom_t;

int chatroom_create();
void chatroom_delete(int id);

void chatroom_send_all(const char msg, chatroom_t* room);

void chatroom_send(const char* msg, int crid);
void chatroom_adduser(int uid, int crid);
void chatroom_removeuser(int uid, int crid);

#endif