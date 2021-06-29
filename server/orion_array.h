#ifndef ORION_ARRAY_H
#define ORION_ARRAY_H
#include "orion.h"


typedef struct {
    unsigned char* data;
    size_t used;
    size_t size;
} orion_array_t;

void array_init(orion_array_t* array, size_t initial_size);
void array_insert(orion_array_t* array, unsigned char data);
void array_free(orion_array_t* array);
#endif