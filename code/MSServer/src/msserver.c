#include "../include/msserver.h"
#include <syslog.h>
/*Variable global con la lista de peliculas*/
/*Es una lista de filmDataT*/
listADT peliculas=NULL;
endPoint infoPl ={0};
endPoint infoLdap ={0};
listADT listSocket;

void
reaper(int sig)
{
    int status;

    while (wait3(&status, WNOHANG, (struct rusage *)0) >= 0)
        /* empty */;
}

int
compInt(int * ps1,int * ps2)
{
    return ps1-ps2;

}

void
cerrarSocket(int sig)
{
  ToBegin(listSocket);
  int * ep;
  while ((ep=GetDato(listSocket))!=NULL)
  {
      close(*ep);
      free(ep);
  }
  listFree(listSocket);
  printf("Servidor cerrado con exito, se cerraron todos los sockets.\n");
  exit(0);
}

int
serverClient(int fd)
{
    srand((int)time(NULL));
    comunicationT leo={0};
    printf("Se conecto un nuevo cliente\n");
    getMsgCom(&leo,fd,BLOCK, NULL);
    /*printf("Opcode:%d\nname:%s\npassword:%s\n",leo.opcode,leo.name,((conUserT*)(leo.opData))->data);*/
    switch(leo.opcode){
    case CON_USR: 					connectUser(&leo,fd);
            								break;

    case PRE_CHG_PASS:			chgPass(&leo,fd);
            								break;

    case DOWNINFO:					downloadInfo(&leo,fd);
            								break;

    case PRE_BUY_MOVIE:			buyMovie(&leo,fd);
            								break;

    case PRE_LIST_CLIENTS:	listClients(&leo,fd);
            								break;

    case LIST_MOVIES:				listMovies(&leo,fd);
            								break;

		case LIST_GEN:					listGens(&leo,fd);
            								break;

    case DISCONNECT:				disconnectUser(&leo,fd);
            								break;

    default: 								fprintf(stderr,"Opcode Invalido %d",leo.opcode);
            								break;
    }
    
		close(fd);

    return 0;
}

void
doTCP(int conectionsock)
{
    struct sockaddr_in fsin;    /* the request from address */
    unsigned int    alen;       /* from-address length      */
    int fd, newsock;
    int aux;
    
    alen = sizeof(fsin);
    newsock = accept(conectionsock, (struct sockaddr *)&fsin, &alen);
    aux = newsock;
    if (conectionsock < 0) 
    {
      if (errno == EINTR)
        return;
      fprintf(stderr,"Error en el accept\n");
    }
  switch (fork()) {
    case 0:     /* child */
      close(conectionsock);
      exit(serverClient(aux));
    default:    /* parent */
      (void) close(newsock);
      break;
    case -1:
      fprintf(stderr,"Error del fork\n");
    }
  return;
}


int
main (int argc, char* argv[])
{
  listSocket=listNew ((int(*)(void*,void*))compInt);
  char *iden = "MovieStore";
  openlog(iden,LOG_CONS,LOG_USER);
  listADT listEndPoints;
  int conectionsock;
  int newsock={0};
  int * sockMemory=NULL;
  /*Variables para la parte de select*/
  
  int fd, nfds;
  fd_set  afds, rfds;           /* readable file descriptors */
  /*-------------------------------*/
  unsigned int    alen;       /* length of client's address   */
  char bufferMydata[PORT_LEN]={0};
  int aux=0;

  system("clear");
  printf("\n\n         Bienvenido al servidor!\n\n");
  
  if( (listEndPoints=getData(&infoPl,&infoLdap))== NULL)
  {
    fprintf(stderr,"Error en el archivo de configuracion\n");
    exit(1);
  }

  /*Salida a pantalla para verificar la entrada del archivo de configuracion*/
  printf("Configuracion cargada:\n");
  printf("ldap Server:\t IP: %s\tpuerto: %d\n",infoLdap.dirIP,infoLdap.numbPort);
  printf("Payment LookUp Server:\t IP: %s\tpuerto: %d\n\n",infoPl.dirIP,infoPl.numbPort);

  endPoint *ep;
  int auxSock;
  char newPort[MAX_NAME];
  (void) signal(SIGCHLD, reaper);
  (void) signal(SIGINT, cerrarSocket);
  ToBegin(listEndPoints);
  nfds = 0;
  FD_ZERO(&afds);
  while ((ep=GetDato(listEndPoints))!=NULL)
  {
      printf("ip: %s \t puerto: %d\n",ep->dirIP,ep->numbPort);
      sprintf(newPort,"%d",ep->numbPort);
      auxSock=passiveTCP(ep->dirIP, newPort, QLEN);
      if(auxSock==PASSIVE_ERROR)
      {
        fprintf(stderr,"Error en la creacion del socket. Reinicie la Aplicacion\n.");
        exit(1);
      }
      sockMemory=malloc(sizeof(int));
      *sockMemory=auxSock;
      listInsert (listSocket, sockMemory);
      nfds = MAX(auxSock+1, nfds);
      free(ep);
      FD_SET(auxSock, &afds);
  }
  
  listFree(listEndPoints);
/*---------------------------------------------------------------------------*/

//   sprintf(bufferMydata,"%d",myPort);

  /*Leo la informacion de la linea de comando*/
  /*Carga del archivo de configuracion de las peliculas disponibles*/
  printf("\nCargando la base de datos de datos de peliculas...\n");

  if(loadFilms()<0)
  {
    fprintf(stderr,"Error en la carga de la base de datos de peliculas\n");
    exit(1);
  }
  printf("\nSe cargo con exito la base de datos de peliculas.\n\n"); 
  /*---------------------------------------------------------------*/

  loadGens();

  initServidor();

  struct sockaddr_in fsin;
  conectionsock = passiveTCP(NULL,bufferMydata, QLEN);
  
  while (1) {
        alen=sizeof(fsin);
        printf("\nEsperando conexiones...\n");
        memcpy(&rfds, &afds, sizeof(afds));
        if (select(nfds, &rfds, (fd_set *)0, (fd_set *)0,(struct timeval *)0) < 0) 
        {
            if (errno == EINTR)
                continue;
            fprintf(stderr,"select error: %s\n", strerror(errno));
        }
        for (fd=0; fd<nfds; ++fd){
          if (FD_ISSET(fd, &rfds)) {
                doTCP(fd);
           }
        }

    }

  return 0;
}

