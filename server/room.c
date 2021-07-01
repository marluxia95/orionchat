#include "room.h"


chatroom_t* chatrooms[MAX_CHATROOMS]; 

pthread_mutex_t chatroom_lock;

int last_crid = 0;


int chatroom_create()
{
    chatroom_t* new_chatroom = (chatroom_t*)malloc(sizeof(chatroom_t));
    new_chatroom->id = last_crid++;
    pthread_mutex_lock(&chatroom_lock);
    for(int id = 0; id < MAX_CHATROOMS; id++){
        if(!chatrooms[id])
            chatrooms[id] = new_chatroom;
    }
    pthread_mutex_unlock(&chatroom_lock);
    return last_crid;
}

void chatroom_delete(int id)
{
    pthread_mutex_lock(&chatroom_lock);
    for(int x = 0; x < MAX_CHATROOMS; x++){
        if(chatrooms[x]->id == id){
            free(&chatrooms[id]);
            chatrooms[id] = NULL;
        }
    }
    pthread_mutex_unlock(&chatroom_lock);
}

void chatroom_send_all(const char msg, chatroom_t* room)
{
    for(int u = 0; u < MAX_CLIENTS_PER_ROOM; u++){
        send_to(msg, room->clients[u]->cfd);
    }
}

void chatroom_send(const char* msg, int crid)
{
    pthread_mutex_lock(&chatroom_lock);

    for(int x = 0; x < MAX_CHATROOMS; x++){
        if (chatrooms[x]->id == crid){
            
        }
    }

    pthread_mutex_unlock(&chatroom_lock);
}

void chatroom_adduser(int uid, int crid)
{

}

void chatroom_removeuser(int uid, int crid)
{

}
