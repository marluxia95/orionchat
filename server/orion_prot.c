#include "orion_prot.h"

#define DEFAULT_ARGCOUNT 1

// Encode data into array
orion_array_t* orion_enc(uint8_t command, size_t arglen, unsigned char* arguments[])
{
    unsigned char* currentArgument;
    orion_array_t* array = (orion_array_t*)malloc(sizeof(orion_array_t) + 2);

    array_init(array, arglen);
    
    array_insert(array, PROTOCOL_VER);
    array_insert(array, command);
    for(int arg = 0; arg < arglen; arg++){
        currentArgument = arguments[arg];
        int size = strlen(currentArgument);
        unsigned char size_1 = (unsigned)size >> 8;
        unsigned char size_2 = (unsigned)size & 0xFF;
        array_insert(array, size_1);
        array_insert(array, size_2);
        for(int c = 0; c < size; c++){
            array_insert(array, currentArgument[c]);
        }
    }

    return array;
}

void orion_clear(char** argdata)
{
    if(!argdata)
        return;

    int argcount = sizeof(argdata) / sizeof(char*);

#ifdef ORION_DEBUG
    puts("ARGARRAY FREE : ");
    printf("Argcount : %d \n", argcount);
#endif

    for(int ar = 0; ar < argcount; ar++) {
        free(argdata[ar]);
    }

    free(argdata);

#ifdef ORION_DEBUG
    puts("argarray free completed.");
#endif
}

// Decode data into an argument array
char** orion_dec(unsigned char* data, size_t data_len)
{
    // Different / Unknown protocol version
    if(data[0] != PROTOCOL_VER)
        return;

    int pos = 0;
    int current_arg = 0;
    int size = data_len;

#ifdef ORION_DEBUG
    for(int x = 0; x < size; x++){
        printf("%x ", data[x]);
    }
    printf("Data size : %d Protocol version : %d Command : %d\n", sizeof(data), data[0], data[1]);
#endif
    pos = 2;
    
    char** argdata = (char**)malloc(DEFAULT_ARGCOUNT * sizeof(char*));

    while ( pos < size ){
        int argsize = (int)(((unsigned)data[pos] << 8) | data[pos+1]);
#ifdef ORION_DEBUG
        printf("Argument %d ; Size : %d\n", current_arg, argsize);
#endif
        pos += 2;


        argdata[current_arg] = (char*)malloc(argsize * sizeof(char) + 10); 
        memset(argdata[current_arg], 0, argsize * sizeof(char) + 10);
        for(int c = 0; c < argsize; c++){
            argdata[current_arg][c] = data[pos];
            pos++;
        }
        argdata[current_arg][argsize+1] = '\0';
        argdata = realloc(argdata, ++current_arg * sizeof(char*));
    }
#ifdef ORION_DEBUG
    puts("Finished decoding");
#endif
    return argdata;
}

// Sends a message
void send_to(const char* content, int cfd)
{
    unsigned char* args[] = {(unsigned char*)content};

    orion_array_t* array = orion_enc(C_MSG, 1, args);

    send_raw(array->data, array->used, cfd);

    array_free(array);
}

// Sends a message ( Server wide )
void ssend_to(const char* content, int cfd)
{
    unsigned char* args[] = {(unsigned char*)content};

    orion_array_t* array = orion_enc(C_RMSG, 1, args);

    send_raw(array->data, array->used, cfd);

    array_free(array);
}

// Sends a message to all clients except the sender ( Server Only ) 
void send_all(const char* content, int cid)
{
    unsigned char* args[] = {(unsigned char*)content};

    orion_array_t* array = orion_enc(C_RMSG, 1, args);

    send_all_raw(array->data, array->used, cid);

    array_free(array);
}
