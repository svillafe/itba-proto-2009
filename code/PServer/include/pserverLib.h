#ifndef _P_SERVER_LIB_H
#define _P_SERVER_LIB_H

#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include <fcntl.h>
#include "../../Sharelibs/include/msgsock.h"
#include "../../Sharelibs/include/listGen.h"

#define MAX_BUFFER 256
#define MAX_TOKEN 32
#define QLEN 32

#define DELIMITER ";"
#define DELIMITER_CHAR ';'

typedef struct
{
  char name[MAX_TOKEN];
  char number[MAX_TOKEN];
  int securityCode;
  double amount;
}psClientInfoT;

//Le la informacion de lo clientes de un archivo
int getData(listADT psClientsList);

//Guarda la informacion de lo clientes en un archivo
int writeData(listADT psClientsList);

//Libera la memoria alocada
void freeMemory(listADT psClientsList);

#endif
