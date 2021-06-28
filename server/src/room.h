#ifndef __ROOM_INCLUDE_
#define __ROOM_INCLUDE_

#include "chadchat.h"

#define MAX_CLIENTS_PER_ROOM 255

typedef struct {
    client_t clients[MAX_CLIENTS_PER_ROOM];

} chatroom_t;


#endif