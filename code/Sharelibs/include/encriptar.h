#ifndef _ENCRIPTAR_H
#define _ENCRIPTAR_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../../des/include/bit.h" 
#include "../../des/include/encrypt.h" 
#define LONG_KEY 8
#define ENCRYPT_ERROR -1
#include "../include/encriptar.h"
int desencriptar(char * resp, char * encriptado,int size, char * desKey);
char * desencriptarWrapper(char * original,int size,char * password);
char * encriptarWrapper(char * original,int size,char * password);
int setKey(char desKey[LONG_KEY], char * password);
int encriptar(char * resp, char * original,int size, char * desKey);
#endif



















