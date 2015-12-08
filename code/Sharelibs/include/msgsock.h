#ifndef _MSG_SOCK_H
#define _MSG_SOCK_H

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <fcntl.h>

#include "opStructs.h"
#include "codes.h"

#define BLOCK 1
#define NO_BLOCK 0

#define WRITE_OK 1
#define READ_ERROR -1
#define ENTER_ERROR -2
#define MEM_ERROR -3
#define READ_OK 1 

#define PLSERVER_MSG_LEN (MAX_NAME+IP_LEN+sizeof(int)+sizeof(int))



typedef struct {
  char dirIP[IP_LEN];
  int numbPort;
} endPoint; 


//Capa aplicacion:
typedef struct{
  int opcode;
  char name[MAX_NAME];
  void * opData;
}comunicationT;

/*Capa de transporte:*/
typedef struct{
comunicationT data;
int magicCookie;
}TransportT;

#include "parse.h"
#include "magicCookie.h"
#include "theReaders.h"
#include "encriptar.h"

int enterMsgCom(comunicationT * msg, int fd, int block,char * password);
char * msgToNetwork(TransportT * msg,int *size);


 /*Lee del socket que le indiquen, la estructura
 *comunicationT utilizado para debug, borrarlo 
 * en un futuro lejano
 */

int receivemsg(comunicationT* resp, int fd);

int writeOnSock(int fd,char * mensaje, int size);

int getMsgCom(comunicationT* resp, int fd,int block, char * psw);

void* readTheSecondBlock(int opCode,int fd,int block,int*mcookie,char * password);
 
int readFromSock(int fd,int size,int block,char *buffer);

int readTextFromSock(int fd,char *buffer,int max_lines,int max);




#endif








































