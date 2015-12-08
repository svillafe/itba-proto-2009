#ifndef _PLSERVER_H_
#define _PLSERVER_H_
//#include "../../MSServer/include/msserverLib.h"
#include "connect.h"
#include "msgsock.h"
#define ISERVER_ERROR -1
#define MAXBUFFER 32
//Estructura que encapsula la info que se manda entre mms y pls
typedef struct{
  char name[MAX_NAME]; //identificador del payment server
  endPoint ep; //el plserver retorna aca la informacion sobre el ps
  int TTL;
}plserverMsgT;

/*Envia un mensaje al payment lookup server*/
int enterPLsrvMsg(plserverMsgT * msg, int fd, int block);

int clientEnterPLsrvMsgUDP(plserverMsgT * msg, int fd, int block);

int enterPLsrvMsgUDP(plserverMsgT * msg, int fd, int block,int flag,struct sockaddr_in * fsin);

/*Recibe un mensaje del payment lookup server*/
int clientGetPLsrvMsgUDP(plserverMsgT * msg, int fd);

int enterPLsrvMsg(plserverMsgT * msg, int fd, int block);

/*Funci�n que inicializa la conexion con el payment lookup server*/
int connectPLServer(endPoint *plData, int *plsocket);

#endif

