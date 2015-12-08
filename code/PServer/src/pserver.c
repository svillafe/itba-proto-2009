#include "../include/pserver.h"

listADT psClientsList=NULL;


static void procRequest(psMsgRequestT *msgReq,psMsgReplyT * msgReply);

int
main(int argc,char **argv)
{
  int myPort=0,conectionsock=0,newsock=0;
  char bufferMyPort[MAX_PORT]={0};
  
  struct  sockaddr_in fsin;   /* the address of a client  */
  unsigned int    alen;       /* length of client's address   */
  
  char *iden = "Payment server";
  openlog(iden,LOG_CONS,LOG_USER);

  if (argc!=2 || (myPort=atoi(argv[1]))<=1024 || myPort>65000)
     myPort=10001; //si no se pasa por linea de comandos se utiliza este default
  
  
  if ( (psClientsList= listNew ( (int(*)(void*,void*))compPsClients))==NULL)
  {
      fprintf(stderr,"Error al iniciar payment server. No se pudo inicializar lista");
      return -1;
  }
  

  printf("Hola. Mi puerto es %d.\n",myPort);


  sprintf(bufferMyPort,"%d",myPort);

  conectionsock = passiveTCP(NULL,bufferMyPort, QLEN);

  (void) signal(SIGCHLD, reaper);

  while (1) 
  {
    printf("Esperando conexiones...\n");
    alen = sizeof(fsin);
    newsock = accept(conectionsock, (struct sockaddr *)&fsin, &alen);
    if (conectionsock < 0)
    {
      if (errno == EINTR)
        continue;
      syslog(LOG_INFO,"Error en el accept");
      fprintf(stderr,"Error en el accept\n");
    }

    switch (fork())
    {
      case -1:
        fprintf(stderr,"Error del fork\n");
        syslog(LOG_INFO,"Error del fork");
        break;
      case 0:     /* child */
        close(conectionsock);
        exit(cltPServer(newsock));
      default:    /* parent */
        (void) close(newsock);
        break;

    }
   }
  
   return 0;

}


int
cltPServer(int fd)
{
    printf("Se conecto alguien\n\n");
    syslog(LOG_INFO,"Se conecto alguien");
 

    psMsgRequestT msgReq={0};
    
    psMsgReplyT msgReply={0};

    if(getData(psClientsList)==-1) //leo la informacion de los clientes
      exit(-1);
  
    if (getPSRequest(&msgReq,fd)==READ_ERROR)
    {
        msgReply.statusCode=1; //request mal formado
        strcpy(msgReply.value,"Request mal formado");
        syslog(LOG_INFO,"Request mal formado");
        enterPSReply(&msgReply,fd);
    }

    printf("Se lee:\n\n") ;

    printf("%s\n",msgReq.clientServer);
    printf("%s\n",msgReq.accountName);
    printf("%s\n",msgReq.accountNumber);
    printf("%d\n",msgReq.securityCode);
    printf("%f\n",msgReq.amount);


    procRequest(&msgReq,&msgReply);

    enterPSReply(&msgReply,fd);

    freeMemory(psClientsList);

    closelog();
    close(fd);
    return 0;

}


static void
procRequest(psMsgRequestT *msgReq,psMsgReplyT * msgReply)
{

  psClientInfoT *client;

  int clientExists=0;


  ToBegin(psClientsList);
  
  while (client=GetDato(psClientsList))
  {
      if (strcmp(client->name,msgReq->accountName)==0)
      {
	  clientExists=1;

	  if (strcmp(client->number,msgReq->accountNumber))
	  {
	      msgReply->statusCode=2;
	      strcpy(msgReply->value,"No coinciden los accountNumbers");
	      break;
	  }

	  if (client->securityCode!=msgReq->securityCode)
	  {
	      msgReply->statusCode=2;
	      strcpy(msgReply->value,"No coinciden los securityCodes");
	      break;
	  }

	  if (client->amount<msgReq->amount)
	  {
	      msgReply->statusCode=3;
	      sprintf(msgReply->value,"El cliente solo cuenta con %.2f",client->amount);
	      break;
	  }

	  client->amount-=msgReq->amount;

	  msgReply->statusCode=0;
	  sprintf(msgReply->value,"La operacion fue realizada con exito. El nuevo monto del cliente %s es %.2f",client->name,client->amount);

	  writeData(psClientsList); //actualiza el archivo

	  break;
      }
  }

  if (!clientExists)
  {
      msgReply->statusCode=2;
      sprintf(msgReply->value,"La cuenta %s es inexistente",msgReq->accountName);
  }

}


void
reaper(int sig)
{
    int status;

    while (wait3(&status, WNOHANG, (struct rusage *)0) >= 0)
        /* empty */;
}


int
compPsClients(psClientInfoT * c1,psClientInfoT * c2)
{
    return strcmp(c1->name,c2->name);
}

