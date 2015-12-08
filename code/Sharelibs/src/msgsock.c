#include "../include/msgsock.h"
#include <errno.h>
int cliente = 0;
int servidor = 0;
extern int mCookie; 
/*
Capa aplicacion:
----------------
#define MAX_SEND 1024;
typedef struct{
int opcode;
char mensaje[MAX_SEND];
}comunicationT;

Capa de transporte:
------------------
typedef struct{
comunicationT data;
int magicCookie;
}TransportT;
*/

/*Manda por el socket que le indiquen la estrutura  
 *comunication que le pasen, en block dice
 *si hace o no el write bloqueante*/
int
enterMsgCom(comunicationT * msg, int fd, int block, char * password)
{
  TransportT mensaje={0};
  int flags;
  int oldFlags;
  int netInt;
  char buffer1[FIRST_MSG]={0};
  char * buffer2=NULL;
  int size=0;
  int agregado = 0;
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

	
  mensaje.data=*msg;
  mensaje.magicCookie=setMagicCookie(msg);
//   printf("La mc seteada: %d\n",mensaje.magicCookie);
  /*printf("Mando la magic Cookie: %d\n",mensaje.magicCookie);*/
  netInt=htonl(msg->opcode);
  memcpy(buffer1,&netInt,sizeof(int));
  memcpy(&buffer1[sizeof(int)],msg->name,MAX_NAME);
  buffer2= msgToNetwork(&mensaje,&size);
  if(msg->opcode== BUY_MOVIE || msg->opcode == CHG_PASS || msg->opcode == LIST_CLIENTS)
  {
    buffer2=encriptarWrapper(buffer2,size, password);
    //printf("Otra vez en el enter\n");
    agregado=LONG_KEY-(size%LONG_KEY);
  }
  writeOnSock(fd,buffer1,FIRST_MSG);

	//printf("Lo que mando encriptado es %s y de tama?o: %d\n", buffer2,size+agregado);

	//printf("fd = %d, buffer2 = %s, size = %d, agregado = %d\n", fd, buffer2, size, agregado);


	if(agregado == 8)
		agregado = 0;


	/*printf("size %d y agregado %d\n",size,agregado);*/
  writeOnSock(fd,buffer2,size+agregado);
	/*perror("hola!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");*/
	//printf("Lo que mando encriptado es %s y de tama?o: %d---\n", buffer2,size+agregado);
  /*------------ ----------------------------*/
  /*Vuelvo a colocar el socket como bloqueante*/
    if(!block)
    {
      if (fcntl(fd, F_SETFL, oldFlags) == -1)
				return ENTER_ERROR;
    }
  /* ---------------------------------------*/
  free(buffer2);

  return WRITE_OK;
}

char *
msgToNetwork(TransportT * msg,int *size)
{
  char * resp;
  switch (msg->data.opcode){
    case CON_USR: resp=parseaCon(msg,size);
                break;
	case PRE_BUY_MOVIE:resp=parseaPreBuyMovie(msg,size);
                break;
	case BUY_MOVIE:resp=parseaBuyMovie(msg,size);
                break;
	case PRE_CHG_PASS:resp=parseaPreChg(msg,size);
                break;
	case CHG_PASS:resp=parseaChg(msg,size);
                break;
    case DOWNINFO:resp=parseaDownInfo(msg,size);
                break;
    case DOWNDATA:resp=parseaDownData(msg,size);
                break;
	case PRE_LIST_CLIENTS:resp=parseaPreListClient(msg,size);
                break;
	case LIST_CLIENTS:resp=parseaListClient(msg,size);
                break;
    case LIST_GEN:resp=parseaListGen(msg,size);
                break;
    case LIST_MOVIES:resp=parseaListMovies(msg,size);
                break;
    case DISCONNECT:resp=parseaDisco(msg,size);
                break;
    
  }
  return resp;
}

 /*Lee del socket que le indiquen, la estructura
 *comunicationT utilizado para debug, borrarlo 
 * en un futuro lejano
 */
int
receivemsg(comunicationT* resp, int fd)
{
  char buf[MAX_STRUCT]={0};
  int cantLeidos;
  TransportT recibo;
  cantLeidos= read(fd, buf, sizeof(buf));
  if(cantLeidos == sizeof(TransportT))
  {
    memcpy(&recibo,buf,sizeof(TransportT));
  }
  return cantLeidos;
}

int
writeOnSock(int fd,char * mensaje, int size)
{
  int resp;
  resp= write(fd,mensaje,size);
  /*if( (resp = send(fd,mensaje,size,0)) != size )
  {
		printf("se pudrio todo en el send.resp %d  size %d\n",resp, size);
    perror("Error:");
  }*/

  return resp;
}

/* Lee del socket que le indican una estructura comunicationT
 * y hace los read bloqueantes o no segun se lo indiquen en 
 * block*/
/*
 *Pasos: 
 *leer el primer bloque
 *segun el opcode leer el segundo
 *y si segun el segundo si me dice, la cantidad del variable
 *
*/
int
getMsgCom(comunicationT* resp, int fd,int block, char * password)
{
	

/*int sizeTransport = sizeof(TransportT);
  char buf[sizeTransport];
  int cantLeidos = 0;
  int leidos = 0;
  TransportT *receive;*/
  int flags;
  int oldFlags;
  int count =0;
  char* buffer1;/*Coloco el primer mensaje*/
  int mcookie=0;
  void * encriptado;
  int size;
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
  
 /* if( (receive = malloc(sizeof(TransportT))) == NULL )
  {
    fprintf(stderr,"\n\nNo hay memoria para el malloc.\n\n");
      return MEM_ERROR;
  }*/
  /*Leo el primer bloque*/
  if((buffer1=malloc(FIRST_MSG))==NULL)
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    exit(1);
  }
  readFromSock(fd,FIRST_MSG,block,buffer1);
	if(buffer1==NULL)
  {
    if(!block)
       fcntl(fd, F_SETFL, oldFlags);
    return READ_ERROR;
  }
  resp->opcode=ntohl(*((int*)&buffer1[0]));
  memcpy(resp->name,&(buffer1[sizeof(int)]),MAX_NAME);
	/*printf("opcode que levanto es: %d, y el name %s\n",resp->opcode,resp->name);*/
  resp->opData=readTheSecondBlock(resp->opcode,fd,block,&mcookie,password);
	

  if(resp->opData==NULL && resp->opcode!=PRE_BUY_MOVIE && resp->opcode!=PRE_LIST_CLIENTS && resp->opcode!=PRE_CHG_PASS)
  {
    if(!block)
       fcntl(fd, F_SETFL, oldFlags);
    return READ_ERROR;
  }
  /*printf("La magic Cookie que recibo es: %d\n",mcookie);*/

  if(!checkMCookie(mcookie))
  {
    if(!block)
       fcntl(fd, F_SETFL, oldFlags);
    return READ_ERROR;
  }
  /*----------------------------------------*/
  /*Vuelvo a colocar el socket como bloqueante*/
  if(!block)
  {
    if (fcntl(fd, F_SETFL, oldFlags) == -1)
      return READ_ERROR;
  }
  /* ---------------------------------------*/
  return READ_OK;  
}

void*
readTheSecondBlock(int opCode,int fd,int block,int*mcookie,char * psw)
{
  void*resp;
  switch(opCode){
    case CON_USR: resp= readConUsr(fd,block,mcookie,psw);
                break;
    case CHG_PASS:resp=readChg(fd,block,mcookie,psw);
                break;
    case PRE_CHG_PASS:resp=readPreChg(fd,block,mcookie,psw);
                break;
    case DOWNDATA:resp=readDownData(fd,block,mcookie,psw);
                break;
    case DOWNINFO:resp=readDownInfo(fd,block,mcookie,psw);
                break;
	case PRE_BUY_MOVIE:resp=readPreBuyMovie(fd,block,mcookie,psw);
                break;
	case BUY_MOVIE:resp=readBuyMovie(fd,block,mcookie,psw);
                break;
    case LIST_GEN:resp=readListGen(fd,block,mcookie,psw);
                break;
    case LIST_MOVIES:resp=readListFilm(fd,block,mcookie,psw);
                break;
    case LIST_CLIENTS:resp=readListClient(fd,block,mcookie,psw);
                break;
    case PRE_LIST_CLIENTS:resp=readPreListClient(fd,block,mcookie,psw);
                break;
    case DISCONNECT:resp=readDisco(fd,block,mcookie,psw);
                break;
  }
  return resp;
}
 
/*
char*
readFromSock(int fd,int size,int block){
  char* resp;
  int cantLeidos=0;
  int leidos=0;
  int count=0;
  if((resp=malloc(size))==NULL)
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    exit(1);
  }
  for(cantLeidos = 0; cantLeidos < size; )
  {
    leidos= read(fd, &resp[leidos], size - cantLeidos);
      if(leidos < 1)
      {
	count ++;
	if(!block && count == 3)
	{
	  return NULL;
	}
      }
      else
	cantLeidos += leidos;
  }
  return resp;
}
*/
int
readFromSock(int fd,int size,int block,char *buffer)
{
  char* resp;
  int cantLeidos=0;
  int leidos=0;
  int count=0;
  int count2=0;
  int resp2;
  /*if((resp2=recv(fd,buffer,size,MSG_WAITALL))!=size)
  {
    printf("Se pudrio todo en el read %d\n",resp2);
  }*/
  for(cantLeidos = 0; cantLeidos < size; )
  {
    leidos= read(fd, &buffer[cantLeidos], size - cantLeidos);
    
	if(cantLeidos==0 && leidos < 1)
	{
        sleep(1);
        count ++;
		if(!block && count == 10)
		{
          return READ_ERROR;
		}
	}
	else if(cantLeidos!=0 && leidos <1)
    {
      count2++; 
      sleep(1);
    }
	else
        cantLeidos += leidos;

    if(count2==5 && !block)
    {
      return READ_ERROR;
    }
  }
	
  return cantLeidos;
}

/*Esta funcion es parecida a readFromSock, y su tarea es la misma, leer un mensaje de un socket, se diferencian en que
 *esta funcion se utiliza cuando el mensaje es del tipo orientado a texto, y por lo cual no se sabe que tan largo es, pero se sabe cuantos 
 '\n' tiene que haber. Si encuentra un '\0' tambien finaliza*/

int
readTextFromSock(int fd,char *buffer,int max_lines,int max)
{
  int cantLeidos=0;
  int leidos=0;
  int cantLineas=0;
  char lastChar='1';

  do
  {
      leidos= read(fd, &buffer[cantLeidos], 1);

      if (leidos>0)
      {
	  lastChar=buffer[cantLeidos];

	  if (lastChar=='\n')
	      cantLineas++;

	  cantLeidos+=leidos;
      }
  }
  while (cantLeidos<max-1 && cantLineas<max_lines && (leidos<1 || lastChar != '\0') );

  return cantLeidos;
}




























