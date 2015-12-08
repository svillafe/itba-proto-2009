#include "../include/msgPLServer.h"
#include <errno.h>
extern int	errno;
struct sockaddr_in fsin;
unsigned int	alen;

int
connectPLServer(endPoint *plData, int *plsocket)
{
    char puertoPl[MAXBUFFER];
    sprintf(puertoPl,"%d",plData->numbPort);
    unsigned int    alen;   
    struct sockaddr_in fsin;    /* the from address of a client */
    alen = sizeof(fsin);
    printf("Intentando conectar el plServer...\n");
    if((*plsocket=connectUDP(plData->dirIP, puertoPl))==CONNECT_ERROR)
    {
      fprintf(stderr,"Error al intentar conectar el plServer.\n\n");
			return ISERVER_ERROR;
    }

    return 0;
}

/*Manda por el socket que le indiquen la estrutura  
 *plserverMsgT al payment lookup server, en block dice
 *si hace o no el write bloqueante*/
int
clientEnterPLsrvMsgUDP(plserverMsgT * msg, int fd, int block)
{
  int flags,port;
  int oldFlags;
  char buffer[PLSERVER_MSG_LEN];
  int a;
	/* ---------------------------------------*/
  /* Coloco el socket como no bloqueante*/
  if(!block)
  {
    if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
			return ENTER_ERROR;
		
    oldFlags= flags;
    flags = flags | O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1)
			return ENTER_ERROR;
		
  }
  /* ---------------------------------------*/
  
  memcpy(buffer,msg->name,MAX_NAME);
  memcpy(&buffer[MAX_NAME],msg->ep.dirIP,IP_LEN);

  port=htonl(msg->ep.numbPort);
  memcpy(&buffer[MAX_NAME+IP_LEN],&port,sizeof(int));

  int ttl=htonl(msg->TTL);

  memcpy(&buffer[MAX_NAME+IP_LEN+sizeof(int)],&ttl,sizeof(int));

  if((a=write(fd, buffer, PLSERVER_MSG_LEN))==-1)
  {
          printf("Error: %s\n",strerror(errno));
          return ENTER_ERROR;
  }
	
  /*------------ ----------------------------*/
  /*Vuelvo a colocar el socket como bloqueante*/
    if(!block)
    {
      if (fcntl(fd, F_SETFL, oldFlags) == -1)
				return ENTER_ERROR;
		
    }
  /* ---------------------------------------*/
  return 0;
}


/*Manda por el socket que le indiquen la estrutura  
 *plserverMsgT al payment lookup server, en block dice
 *si hace o no el write bloqueante*/
int
serverEnterPLsrvMsgUDP(plserverMsgT * msg, int fd, int block)
{
  int flags,port;
  int oldFlags;
  char buffer[PLSERVER_MSG_LEN];
  int a;
  struct sockaddr_in aux;
  unsigned int    alen;   
  alen = sizeof(fsin);
  /* ---------------------------------------*/
  /* Coloco el socket como no bloqueante*/
  if(!block)
  {
    if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
			return ENTER_ERROR;
		
    oldFlags= flags;
    flags = flags | O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1)
			return ENTER_ERROR;
		
  }
  /* ---------------------------------------*/
  
  memcpy(buffer,msg->name,MAX_NAME);
  memcpy(&buffer[MAX_NAME],msg->ep.dirIP,IP_LEN);

  port=htonl(msg->ep.numbPort);
  memcpy(&buffer[MAX_NAME+IP_LEN],&port,sizeof(int));
  
  int ttl=htonl(msg->TTL);
  memcpy(&buffer[MAX_NAME+IP_LEN+sizeof(int)],&ttl,sizeof(int));
  
  if((a=sendto(fd, buffer, PLSERVER_MSG_LEN, 0,(struct sockaddr *)&fsin, sizeof(fsin)))<0)
  {
    printf("Error: %s\n",strerror(errno));
    return ENTER_ERROR;
  }
	
	
  /*------------ ----------------------------*/
  /*Vuelvo a colocar el socket como bloqueante*/
    if(!block)
    {
      if (fcntl(fd, F_SETFL, oldFlags) == -1)
				return ENTER_ERROR;
		
    }
  /* ---------------------------------------*/
  return 0;
}

int
clientGetPLsrvMsgUDP(plserverMsgT * msg, int fd)
{
  int flags;
  int oldFlags;
  char buffer[PLSERVER_MSG_LEN];
  int port,ttl;
  /* ---------------------------------------*/
    if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
      return READ_ERROR;
    oldFlags= flags;
    flags = flags | O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1)
      return READ_ERROR;
  
  /* ---------------------------------------*/
  if(readFromSock(fd,PLSERVER_MSG_LEN,0,buffer)==-1)
  {
    return READ_ERROR;
  }

  memcpy(msg->name,buffer,MAX_NAME);
  memcpy(msg->ep.dirIP,buffer+MAX_NAME,IP_LEN);
  memcpy(&port,buffer+MAX_NAME+IP_LEN,sizeof(int));
  memcpy(&ttl,buffer+MAX_NAME+IP_LEN+sizeof(int),sizeof(int));
  msg->ep.numbPort=ntohl(port);
  msg->TTL=ntohl(ttl);

  /*-----------------------------------------*/
  /*Vuelvo a colocar el socket como bloqueante*/
    if (fcntl(fd, F_SETFL, oldFlags) == -1)
	   return READ_ERROR;
  /* ---------------------------------------*/

  return 0;
}

int
serverGetPLsrvMsgUDP(plserverMsgT * msg, int fd, int block)
{
  int flags;
  int oldFlags;
  char buffer[PLSERVER_MSG_LEN];
  int port,ttl;
  /* ---------------------------------------*/
  /* Coloco el socket como no bloqueante*/
  if(!block)
  {
    if ((flags = fcntl(fd, F_GETFL, 0)) == -1)
      return READ_ERROR;
    oldFlags= flags;
    flags = flags | O_NONBLOCK;
    if (fcntl(fd, F_SETFL, flags) == -1)
      return READ_ERROR;
  }
  /* ---------------------------------------*/
  if (recvfrom(fd, buffer, PLSERVER_MSG_LEN, MSG_WAITALL, (struct sockaddr *)&fsin, &alen) < 0)
  {	
 		return READ_ERROR;
  }
	
  memcpy(msg->name,buffer,MAX_NAME);
  memcpy(msg->ep.dirIP,buffer+MAX_NAME,IP_LEN);
  memcpy(&port,buffer+MAX_NAME+IP_LEN,sizeof(int));
  memcpy(&ttl,buffer+MAX_NAME+IP_LEN+sizeof(int),sizeof(int));
  
  msg->ep.numbPort=ntohl(port);
  msg->TTL=ntohl(ttl);
	/*------------ ----------------------------*/
  /*Vuelvo a colocar el socket como bloqueante*/
    if(!block)
    {
      if (fcntl(fd, F_SETFL, oldFlags) == -1)
				return READ_ERROR;
    }
  /* ---------------------------------------*/
  return 0;
}






