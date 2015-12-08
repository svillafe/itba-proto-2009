#ifndef _P_L_SERVER_LIB_H
#define _P_L_SERVER_LIB_H

#define LINE_MSS_FILE 2
#define MAX_BUFFER 124
#define MAX_TOKEN 64
#define QLEN 32

#define DELIMITER ";"
#define DELIMITER_CHAR ';'

#include <stdio.h>
#include <string.h>
#include <sys/errno.h>
#include "../../Sharelibs/include/msgPLServer.h"
#include "../../Sharelibs/include/msgsock.h"
#include "../../Sharelibs/include/listGen.h"


//lee la informacion de archivo sobre los payment servers
int getData(listADT psList);

//Busca cual es el ps que corresponde
void searchPServer(plserverMsgT *msg,listADT psList);

//Libera la memoria alocada
void freeMemory(listADT psList);

#endif
