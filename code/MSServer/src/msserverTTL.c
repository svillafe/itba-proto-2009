
#include "../include/msserverTTL.h"
int
saveTTL(plserverMsgT * psData)
{
  int output=0;
  char buffer[MAX_BUFFER_TTL]={0};
  char line[MAX_BUFFER_TTL]={0};
  char nombre[MAX_NAME+10]={0};
  sprintf(nombre,"%sTTL",psData->name);
  if((output=open(nombre,O_WRONLY|O_TRUNC|O_CREAT,0666))==-1)
  {
	  fprintf(stderr,"No se pudo abrir el archivo %s, para grabar.\n",nombre);
      perror("ERROR:");
	  return -1;
  }  

  //Aplico lock al archivo para que nadie mas lo pueda cambiar mientras se esta actualizando

  struct flock lock;

  lock.l_type = F_RDLCK;
  lock.l_whence = SEEK_SET;
  lock.l_start = 0;
  lock.l_len = 0;

  fcntl(output,F_SETLKW,&lock);
	sprintf(line,"%d\n",psData->TTL);
  write(output,line,strlen(line));
  memset(line,'\0',sizeof(line));

	sprintf(line,"%s\n",psData->ep.dirIP);
  write(output,line,strlen(line));
	memset(line,'\0',sizeof(line));

	sprintf(line,"%d\n",psData->ep.numbPort);
  write(output,line,strlen(line));
	memset(line,'\0',sizeof(line));

	sprintf(line,"%d\n",(int)time(NULL));
  write(output,line,strlen(line));
	memset(line,'\0',sizeof(line));

  lock.l_type=F_UNLCK;

  fcntl(output,F_SETLKW,&lock);	//desbloqueo archivo

  close(output);
  return 0;
}


/*Foramato del archivo
*	TTL (texto)
*	IP (texto)
* Puerto (texto)
* inicio (texto)
*/

int 
getPSEndpoint(endPoint * resp, char * name)
{
  FILE * input=NULL;
  char buffer[MAX_BUFFER_TTL]={0};
  char psName[MAX_NAME+10]={0};
  int numLinea=1;
  int TTL;
  int comienzo;
  endPoint tentResp;
  sprintf(psName,"%sTTL",name);
  if( (input=fopen(psName,"r"))==NULL)
  {
	  fprintf(stderr,"No se pudo abrir el archivo %s, para cargar.\n",psName);
      return 0;
  }  

  while(fgets(buffer,MAX_BUFFER_TTL-1,input)!=NULL && *buffer!='\n' )
  {
      buffer[strlen(buffer)-1]=0;
      switch(numLinea){
			case 1:if(sscanf(buffer,"%d",&(TTL))!=1)
			{
			 fprintf(stderr,"Error: archivo %s mal formado.\n",psName);
			 fclose(input);
			 return 0;
			}
			break;
			case 2:strcpy(tentResp.dirIP,buffer); /*Falta chequear que se aIP valida*/
							break;
			case 3:if(sscanf(buffer,"%d",&(tentResp.numbPort))!=1)
							{
								fprintf(stderr,"Error: archivo %s mal formado.\n",psName);
								fclose(input);
								return 0;
							}
							break;
			case 4:if(sscanf(buffer,"%d",&(comienzo))!=1)
							{
								fprintf(stderr,"Error: archivo %s mal formado.\n",psName);
								fclose(input);
								return 0;
							}
							break;
			default:
						{
								fprintf(stderr,"Error: archivo %s mal formado.\n",psName);
								fclose(input);
								return 0;
							}
			}
			numLinea ++;
	}
	if(!(numLinea==5 || (numLinea==6 && *buffer=='\n')))
	{
		fprintf(stderr,"Error: archivo %s mal formado.\n",psName);
		fclose(input);
		return 0;
	}
	fclose(input);

	strcpy(resp->dirIP,tentResp.dirIP);
	resp->numbPort=tentResp.numbPort;
//     printf("TTL: %d\n",TTL);
//     printf("Dir ip: %s\n",tentResp.dirIP);
//     printf("Numero de puerto: %d\n",tentResp.numbPort);
//     printf("el comienzo que levanto es: %d\n",comienzo);
//     printf("La resta da: %d\n",((int)time(NULL)-comienzo));
	if(((int)time(NULL)-comienzo)>TTL)
    {
		  return 0;
    }
	return 1;
}

