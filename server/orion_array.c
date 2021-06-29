#include "orion_array.h"

void array_init(orion_array_t* array, size_t initial_size)
{
    array = (orion_array_t*)malloc(initial_size);
    array->used = 0;
    array->size = initial_size;
}

void array_insert(orion_array_t* array, unsigned char data)
{
    if(array->used == array->size){
        size_t new_size = array->size+sizeof(data)+10;
        realloc(array, new_size);
        array->size = new_size;
    }
    array->data[array->used++] = data;
}

void array_free(orion_array_t* array)
{
    free(array->data);
    array->data = NULL;
    array->size = array->used = 0;
}