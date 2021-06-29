#include "orion_enc.h"


orion_array_t* orion_enc(uint8_t command, size_t arglen, unsigned char* arguments[])
{
    orion_array_t* array = (orion_array_t*)malloc(sizeof(orion_array_t));

    array_init(array, arglen);
    array_insert(array, PROTOCOL_VER);
    array_insert(array, command);
    for(int arg = 0; arg < arglen; arg++){
        array_insert(array, sizeof(arguments[arg]));
        for(int c = 0; c < sizeof(arguments[arg]); c++){
            array_insert(array, arguments[arg][c]);
        }
    }
}

void orion_dec(orion_array_t* array, char* arguments[])
{
    // Different / Unknown protocol version
    if(array->data[0] != PROTOCOL_VER)
        return;

    
}