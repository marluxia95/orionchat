#ifndef ORION_ENC_H
#define ORION_ENC_H

#include "orion.h"
#include "orion_array.h"
#include "orion_enc.h"

//
//  Understanding the protocol
// 
//  Byte   1     : The protocol version 
//  Byte   2     : Command 
//  Bytes  3-4   : Size of first argument ( 2 bytes )
//  Bytes  4 - ? : First argument's data
//
//  Protocol version
//  |      Size of first argument ( 2 bytes )
//  V      V                
// 01 02 00 08 48 65 6C 6C 6F 00 57 00 6F 08 57 6F 72 6C 64 00 49 6E
//    ^                               ^
//    Command                         Size of second argument ( 2 bytes )
//
// Maximum size depends on client's buffer size.
// 
// 

#define PROTOCOL_VER 1

orion_array_t* orion_enc(uint8_t command, size_t arglen, unsigned char* arguments[]);
void orion_dec(unsigned char* data, size_t data_len, char* arguments[]);

#endif