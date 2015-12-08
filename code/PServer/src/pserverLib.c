#include "../include/pserverLib.h"

static int checkHeader(char *buffer);
static psClientInfoT * getClientInfo(char *buffer);

int
getData(listADT psClientsList)
{
  FILE * input=NULL;
  char buffer[MAX_BUFFER]={0};
  psClientInfoT *client=NULL;
  int numLinea=1;

  if( (input=fopen("Config/psconfig","r"))==NULL)
  {
	  fprintf(stderr,"No se pudo abrir el archivo psconfig.\n");
	  return -1;
  }  

  if (fgets(buffer,MAX_BUFFER-1,input)==NULL || checkHeader(buffer))
  {
	  fprintf(stderr,"Error en el archivo psconfig.\n");
	  fclose(input);
	  return -1;
  }



  while(fgets(buffer,MAX_BUFFER-1,input)!=NULL && *buffer!='\n' )
  {
      numLinea++;
      if ((client=getClientInfo(buffer))==NULL)
      {
	      fprintf(stderr,"Error en el archivo psconfig. Linea %d\n",numLinea);
	      fclose(input);
	      freeMemory(psClientsList);
	      return -1;
      }
      
     listInsert (psClientsList, client);
  }

  fclose(input);

  return 0;
}

static int
checkHeader(char *buffer)
{
  char * aux=NULL;
  int i=0;
  int length=0;
	
  length=strlen(buffer);

  if (buffer[length-1]=='\n')
	  buffer[length-1]=DELIMITER_CHAR;
  else
  {
	  buffer[length]=DELIMITER_CHAR;
	  buffer[length+1]='\0';
  }

  char * header[]={"AccountName","AccountNumber","SecurityCode","Amount"};

  if ((aux=strtok(buffer,DELIMITER))==NULL || strcmp(aux,header[0]))
      return -1;
  
  for (i=1;i<4;i++)
  {
    if ((aux=strtok(NULL,DELIMITER))==NULL || strcmp(aux,header[i]))
	return -1;
  }

  return 0;
}
 
static psClientInfoT *
getClientInfo(char *buffer)
{
  char * aux=NULL;
  int i=0;
  int length=0;
  psClientInfoT *client=NULL;	
  
  length=strlen(buffer);

  if (buffer[length-1]=='\n')
	  buffer[length-1]=DELIMITER_CHAR;
  else
  {
	  buffer[length]=DELIMITER_CHAR;
	  buffer[length+1]='\0';
  }

  if ( (client = malloc(sizeof(psClientInfoT)))==NULL)
  {
	fprintf(stderr,"Error al pedir memoria dinamica\n");
	return NULL;
  }

  if ((aux=strtok(buffer,DELIMITER))==NULL || strlen(aux)>=MAX_TOKEN)
      return NULL;

  strcpy(client->name,aux);

  if ((aux=strtok(NULL,DELIMITER))==NULL || strlen(aux)>=MAX_TOKEN)
	return NULL;
 
  strcpy(client->number,aux);

  if ((aux=strtok(NULL,DELIMITER))==NULL || (client->securityCode=atoi(aux))==0)
	return NULL;

  if ((aux=strtok(NULL,DELIMITER))==NULL || (client->amount=atof(aux))==0)
	return NULL;

  return client;
}

int
writeData(listADT psClientsList)
{
  int output=0;
  char buffer[MAX_BUFFER]={0};
  psClientInfoT *client=NULL;
  int numLinea=1;
  char line[MAX_BUFFER]={0};

  if( (output=open("Config/psconfig",O_WRONLY|O_TRUNC,777))==-1)
  {
	  fprintf(stderr,"No se pudo abrir el archivo psconfig.\n");
	  return -1;
  }  

  //Aplico lock al archivo para que nadie mas lo pueda cambiar mientras se esta actualizando

  struct flock lock;

  lock.l_type = F_RDLCK;
  lock.l_whence = SEEK_SET;
  lock.l_start = 0;
  lock.l_len = 0;

  fcntl(output,F_SETLKW,&lock);

  sprintf(line,"AccountName;AccountNumber;SecurityCode;Amount\n");

  write(output,line,strlen(line));

  memset(line,'\0',sizeof(line));

  ToBegin(psClientsList);

  while (client=GetDato(psClientsList))
  {
      sprintf(line,"%s;%s;%d;%.2f\n",client->name,client->number,client->securityCode,client->amount);
      write(output,line,strlen(line));
      memset(line,'\0',sizeof(line));

  }

  lock.l_type=F_UNLCK;

  fcntl(output,F_SETLKW,&lock);	//desbloqueo archivo

  close(output);
  return 0;
}


void
freeMemory(listADT psClientsList)
{
  psClientInfoT *client=NULL;

  ToBegin(psClientsList);

  while (client=GetDato(psClientsList))
  {
      free(client);
  }

  listFree(psClientsList);

}

