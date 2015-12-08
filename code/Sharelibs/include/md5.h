#ifndef _MD5_H_
#define _MD5_H_
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <openssl/md5.h>
#define BLOQUE 10
#define LONG_BUFFER_MD5 31
#define MAX_LEN_MD5 31
unsigned char * getMd5(char * name);
char *  md5ToString(const unsigned char * md5);
void eraseMd5File(char * name);
#endif










