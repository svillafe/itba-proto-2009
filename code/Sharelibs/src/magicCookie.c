#include "../include/magicCookie.h"
int mCookie=0; 
extern int cliente;
extern int servidor;

/*Graba la Magic Cookie una ves terminada una tarea del servidor*/

void
realeaseServer(char * nombre)
{
  char * buffer;
  char * aux= "mc";
  int fd;
	if(strcmp(nombre,"Invitado")==0)
	{
		return;
	}
	if((buffer = malloc(strlen(nombre)+strlen(aux)+1))== NULL)
  {
    fprintf(stderr,"Error de memoria\n");
    return;
  }
  sprintf(buffer,"%s%s",nombre,aux);
  if((fd=open(buffer,O_RDWR|O_CREAT,0666))==-1)
  {
    free(buffer);
    return;
  }
  write(fd,&mCookie, sizeof(mCookie));
	close(fd);
	fd=open(buffer,O_RDWR|O_CREAT,0666);
	free(buffer);
  close(fd);
}

/* Se encarga de leer el archivo de la MCookie y comparar si la mc que
 * le llego es la correcta*/

int
initSerCon(char * userName)
{
  char * buffer;
  char * aux= "mc";
  int nro;
  int fd;
  
	if(strcmp(userName,"Invitado")==0)
	{
		return 1;
	}
	
	if((buffer = malloc(strlen(userName)+strlen(aux)+1))== NULL)
  {
    fprintf(stderr,"Error de memoria\n");
    return 0;
  }
  sprintf(buffer,"%s%s",userName,aux);
  if((fd=open(buffer,O_RDONLY,0666))==-1)
  {
    fprintf(stderr,"Error al abrir el archivo de MC\n");
		free(buffer);
    return 0;
  }
	free(buffer);
  if(read(fd,&nro, sizeof(nro))>0)
  {
		close(fd);
		return mCookie == clientFun(nro);
	}
	else
	{
		fprintf(stderr,"Error al leer el archivo de MC\n");
		return 0;
	}
}

/*Funciones que inicializan cosas en la capa de "transporte" tanto 
 *del servidor como en el cliente*/
void
initCliente(void)
{
	cliente = 1;	
}
void
initServidor(void)
{
	servidor =1;
}


/*Funciones del servidor y del cliente*/
/*------------------------------------*/
int
serverFun(int oldMaCookie)
{
  /*Por ahora manda lo mismo que recibe*/
  return oldMaCookie;
}

int
clientFun(int oldMaCookie)
{
  /*Por ahora manda lo mismo que recibe*/
  return oldMaCookie;  
}
/*--------------------------------------*/
/*Devuelve nueva una Magic Cookie segun sea cliente o servidor*/

int
setMagicCookie(comunicationT * estructura)
{
  int n;
  switch(estructura->opcode){
  case CON_USR: 
		if(cliente)
			return mCookie = rand();
    else if(servidor)
			return mCookie = serverFun(mCookie); 
    break;
	default:  
		if(cliente)
			return mCookie = clientFun(mCookie);
     else if(servidor)
			return mCookie = serverFun(mCookie);
     break;
  }
}

/*Controla la Magic Cookie recibida*/

int
checkMCookie(int mc) 
{
  if(cliente)
  {
    if(mc == serverFun(mCookie))
    {
      mCookie=mc;
      return 1;
    }
    else
      return 0;
  }
  if(servidor)
  {
    if(mCookie==0)
    {
      mCookie=mc;
      return 1;
    }
    if(mc == clientFun(mCookie))
    {
      mCookie=mc;
      return 1;
    }
    else
      return 0;
  }
}





