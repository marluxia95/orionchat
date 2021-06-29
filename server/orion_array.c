#include "orion_array.h"

void array_init(orion_array_t* array, size_t initial_size)
{
    array->data = malloc(sizeof(unsigned char) * initial_size);
    array->used = 0;
    array->size = initial_size;
}

void array_insert(orion_array_t* array, unsigned char data)
{
    if(array->used == array->size){
        array->size *= 2;
        array->data = realloc(array->data, array->size * sizeof(unsigned char));
    }
    array->data[array->used++] = data;
}

void array_free(orion_array_t* array)
{
    free(array->data);
    array->data = NULL;
    array->size = array->used = 0;
    free(array);
}

void array_debug_print(orion_array_t* array)
{
    if(!array)
        return;
    
    printf("Array size : %d Used : %d Data size : %d \n", array->size, array->used, sizeof(array->data));
    for(int x = 0; x < array->used; x++){
        printf("%x ", array->data[x]);
    }
    printf("\n");
}