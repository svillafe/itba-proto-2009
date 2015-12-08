/*en este archivo estan las funciones que usan tanto el cliente como el servidor*/
#include "../include/slib.h"

commandT
getCommand(char *prompt,char* param, commandMenuT * cmt, int size)
{
	char line[MAX_LEN+1];
	char paramRes[MAX_LEN+1]={0};

	char * cmd,*aux;
	commandT command;
	int i=0,k=0,c,j=0,len;

	printf("%s",prompt);

	//almaceno toda la linea	
	while( ((c=getchar())!= '\n') && i<MAX_LEN )	
		line[i++]=c;

	//borra el buffer
	while (c!='\n')
		c=getchar(); 

	if (i==0)
	    return ENTER;

	line[i]='\0';

	cmd=strtok(line," "); //Guardo el comando

	for(j=0 ; j<2 && (aux=strtok(NULL," "))!=NULL; j++)   
	{
		if(j==0)
		{
			len=strlen(aux); //Longitud del primer argumento
			paramRes[k++]=len;
		}
	    	strcpy(paramRes+k,aux); //Guardo los comandos
		k+=len+1;
	}

	//Valido que sea correcta la cantidad de parametros
	for(k=0; k<size ; k++)
		if( strcmp(cmd,cmt[k].name)==0)
		{
			if ( cmt[k].cparam != j )
				return ERPARAM; //cantidad de parametros invalida
			else
			{
			  	memcpy(param,paramRes,MAX_LEN+1);
			  	return cmt[k].command;
			}
		}
	return INVALID_CMD; //Comando ivalido
}

char*
toLower(char *movieName)
{
	int i=0;
	char * minuscula;
	if( (minuscula=malloc(strlen(movieName)+1)) == NULL )
	{
		fprintf(stderr,"Error fatal por falta de memoria\n\n");
		return NULL;	
	} 

	for(i=0; i<strlen(movieName) ;i++)
		minuscula[i] = tolower(movieName[i]);
	
	minuscula[i] = '\0';

	return minuscula;
}


int
getYesNo(char *msg)
{
  int c;
  int res;
  
  do
  {	
        printf("%s", msg );
	res = tolower(getchar());
    
	c = res;

	while (c!='\n')
	  c = getchar();
  }
  while (res!='s' && res!='n');

  return res=='s'; 
}

int 
checkIP(char*buffer)
{
  int pri,seg,ter,cua;
  return (sscanf(buffer,"%d.%d.%d.%d",&pri,&seg,&ter,&cua)==4 && 
          pri <=255 && pri >= 0 && seg <= 255 && seg >=0 && 
          ter <= 255 && ter >= 0 && cua <= 255 && cua >= 0);
}

int
getIP(char *dir)
{
  int i;
  
  if(fgets(dir,MAX_LEN-1,stdin) != NULL)
  {
    for(i=0; i<strlen(dir); i++)
      if(dir[i] == '\n')
				dir[i] == '\0';

    if(checkIP(dir) == 0)
    {
      fprintf(stderr,"\nEL IP INGRESADO NO ES VALIDO\n");
      return 0;
    }
    return 1;
  }

  fprintf(stderr,"\n\nERROR AL LEER IP\n\n");
  return 0;
}

int 
getPort(char *port)
{
	int i, portN;
	
	if(fgets(port,MAX_LEN-1,stdin) != NULL)
	{
		for(i=0; i<MAX_LEN-1; i++)
			if(port[i] == '\n')
				port[i] == '\0';
  
		portN = atoi(port);
		
		if(portN < 1024 || portN > 65000)
		{
			fprintf(stderr,"\n\nError, numero de puerto invalido.\n\n");
			return 0;
		}
		
		return 1;
	}
	fprintf(stderr,"\n\nNO SE PUDO LEER EL PUERTO\n\n");
	return 0;

}
