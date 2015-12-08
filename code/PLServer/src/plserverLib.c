#include "../include/plserverLib.h"

static plserverMsgT* getPSinfo(char *buffer);

int
getData(listADT psList)
{
  FILE * input;
  char buffer[MAX_BUFFER];
  plserverMsgT *ps=NULL;
  int numLinea=0;

  if((input=fopen("Config/plsconfig","r"))==NULL)
  {
	  fprintf(stderr,"No se pudo abrir el archivo de configuracion plsconfig.\n");
	  return -1;
  }
	
  while(fgets(buffer,MAX_BUFFER-1,input)!=NULL && *buffer!='\n')
  {
	numLinea++;

	if ((ps=getPSinfo(buffer))==NULL)
	{
		fprintf(stderr,"Error en el archivo plsconfig. Linea: %d\n",numLinea);
		fclose(input);
		freeMemory(psList);
		return -1;
	}
	
	listInsert (psList, ps);
  }

  fclose(input);

  return 0;
}


static plserverMsgT *
getPSinfo(char *buffer)
{
  char * aux;
  int i;
  int length;
  plserverMsgT *ps;	
  
  length=strlen(buffer);

  if (buffer[length-1]=='\n')
	  buffer[length-1]=DELIMITER_CHAR;
  else
  {
	  buffer[length]=DELIMITER_CHAR;
	  buffer[length+1]='\0';
  }

  if ( (ps = malloc(sizeof(plserverMsgT)))==NULL)
  {
	fprintf(stderr,"Error al pedir memoria dinamica\n");
	return NULL;
  }

  if ((aux=strtok(buffer,DELIMITER))==NULL || strlen(aux)>=MAX_TOKEN)
      return NULL;

  strcpy(ps->name,aux);

  if ((aux=strtok(NULL,DELIMITER))==NULL || !checkIP(aux))
	return NULL;
 
  strcpy(ps->ep.dirIP,aux);

  if ((aux=strtok(NULL,DELIMITER))==NULL || (ps->ep.numbPort=atoi(aux))<=1024 || ps->ep.numbPort>65000)
	return NULL;

  if ((aux=strtok(NULL,DELIMITER))==NULL || (ps->TTL=atoi(aux))==0)
	return NULL;

  return ps;

}

void
searchPServer(plserverMsgT *msg,listADT psList)
{
  plserverMsgT *ps;

  ToBegin(psList);
  int flag=0;

  while (ps=GetDato(psList))
  {
      if (strcmp(ps->name,msg->name)==0)
      {
	        flag=1;
          msg->ep=ps->ep;
          msg->TTL=ps->TTL;
      }
  }
 
  if (!flag)
      strcpy(msg->name,"ERROR");
  
}

void
freeMemory(listADT psList)
{
  plserverMsgT *ps=NULL;

  ToBegin(psList);

  while (ps=GetDato(psList))
  {
      free(ps);
  }

  listFree(psList);
}
