#ifndef ORION_PROT_H
#define ORION_PROT_H

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "orion_array.h"
#include "orion_core.h"

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

//
//  Commands
//

#define C_HELLO 0x01
#define C_NICK  0x02
#define C_PKEY  0x03
#define C_KEY   0x04
#define C_RMSG  0x05
#define C_MSG   0x06
#define C_TYPE  0x07

// 
//  Errors
//

#define ERR_WRONG_NICK 0xA0

#define PROTOCOL_VER 1

orion_array_t* orion_enc(uint8_t command, size_t arglen, unsigned char* arguments[]);
char** orion_dec(unsigned char* data, size_t data_len);

void send_to(const char* content, int cfd);
void ssend_to(const char* content, int cfd);
void send_all(const char* content, int cfd);

#endif