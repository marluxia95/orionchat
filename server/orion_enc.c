#include "orion_enc.h"


orion_array_t* orion_enc(uint8_t command, size_t arglen, unsigned char* arguments[])
{
    orion_array_t* array = (orion_array_t*)malloc(sizeof(orion_array_t));

    array_init(array, arglen);
    
    array_insert(array, PROTOCOL_VER);
    array_insert(array, command);
    for(int arg = 0; arg < arglen; arg++){
        int size = sizeof(arguments[arg]);
        unsigned char size_1 = (unsigned)size >> 8;
        unsigned char size_2 = (unsigned)size & 0xFF;
        array_insert(array, size_1);
        array_insert(array, size_2);
        for(int c = 0; c < sizeof(arguments[arg]); c++){
            array_insert(array, arguments[arg][c]);
        }
    }

    return array;
}

// Decode data into array
void orion_dec(unsigned char* data, size_t data_len, char* arguments[])
{
    // Different / Unknown protocol version
    if(data[0] != PROTOCOL_VER)
        return;

    int pos = 0;
    int current_arg = 0;
    int size = data_len;
    printf("Data size : %d Protocol version : %d Command : %d\n", sizeof(data), data[0], data[1]);
    pos = 2;
    

    while ( pos < size ){
        int argsize = (int)(((unsigned)data[pos] << 8) | data[pos+1]);
        printf("Argument %d ; Size : %d\n", current_arg, argsize);
        pos += 2;
        arguments[current_arg] = (char*)malloc(argsize);
        for(int c = 0; c < argsize; c++){
            arguments[current_arg][c] = data[pos];
            pos++;
        }
        current_arg++;
    }
}