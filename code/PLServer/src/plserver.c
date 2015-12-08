#include "../include/plserver.h"



listADT psList=NULL;
extern int  errno;
struct sockaddr_in fsin;
unsigned int	alen;



/*------------------------------------------------------------------------
 * main - Iterative UDP server for TIME service
 *------------------------------------------------------------------------*/
int sock=-1;
int
main(int argc, char *argv[])
{
  char    buf[PLSERVER_MSG_LEN]={0};             /* "input" buffer; any size > 0 */
  int myPort,conectionsock,newsock;
  char bufferMyPort[MAX_PORT]={0};
  
  plserverMsgT msg;
 
  if ( (psList= listNew ( (int(*)(void*,void*))compPservers))==NULL)
  {
      fprintf(stderr,"Error al iniciar plserver. No se pudo inicializar lista");
      return -1;
  }
 
  if (argc!=2 || (myPort=atoi(argv[1]))<=1024 || myPort>65000)
     myPort=10539; //si no se pasa por linea de comandos se utiliza este default

  if(getData(psList)==-1) //lee la informacion de los Payment servers
      exit(-1);
/*---------------------------------------------*/
  printf("Hola. Mi puerto es %d.\n",myPort);

  plserverMsgT *ps;

  ToBegin(psList);
  
  while (ps=GetDato(psList))
  {
      printf("ID: %s \t ip: %s \t puerto: %d \t TTL: %d\n",ps->name,ps->ep.dirIP,ps->ep.numbPort,ps->TTL);
  }

  sprintf(bufferMyPort,"%d",myPort);

  
  
/*-----------------------------------------------------------*/
  sock = passiveUDP(NULL,bufferMyPort);

  (void) signal(SIGCHLD, reaper);
  while (1) 
  {
    printf("Esperando conexiones...\n");
    alen = sizeof(fsin);

    if(serverGetPLsrvMsgUDP(&msg, sock)<0)
    {
        fprintf(stderr,"Error en el recvfrom\n");
    }
    switch (fork())
    {
      case -1:
        fprintf(stderr,"Error del fork\n");
        break;
      case 0:     /* child */
        exit(cltLookUpSrv(msg));
      break;        
     }
   }

   return 0;
}



int
cltLookUpSrv(plserverMsgT msg)
{
    printf("Se conecto alguien\n");
    //Busca cual el el payment server que corresponde
    searchPServer(&msg,psList);

    //retorna la respuesta al Movie Store Server

    serverEnterPLsrvMsgUDP(&msg,sock,1);

    freeMemory(psList);
    
    return 0;
}

void
reaper(int sig)
{
    int status;

    while (wait3(&status, WNOHANG, (struct rusage *)0) >= 0)
        /* empty */;
}


int
compPservers(plserverMsgT * ps1,plserverMsgT * ps2)
{
	return strcmp(ps1->name,ps2->name);

}

