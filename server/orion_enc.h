#ifndef ORION_ENC_H
#define ORION_ENC_H

#include "orion.h"

//
//  Understanding the protocol
// 
//  Byte  1 : The protocol version 
//  Byte  2 : Command 
//  Byte  3 : Size of first argument
//  Bytes 4 - ? : First argument's data
//
//  Protocol version
//  |     Size of first argument
//  V     V                
// 01 1F 05 0A 0B 0C 0D 0E 06 0A 0B 0C 0D 0E 0F
//    ^                    ^
//    Command              Size of second argument
//
// Maximum size depends on client's buffer size.
// 
// 

#define PROTOCOL_VER 1

orion_array_t* orion_enc(uint8_t command, size_t arglen, unsigned char* arguments[]);
void orion_dec(orion_array_t* array, char* arguments[]);

#endif