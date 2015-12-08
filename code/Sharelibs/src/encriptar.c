#include "../include/encriptar.h"

char*
encriptarWrapper(char * original,int size,char * password)
{
  char * resp;
  char desKey[LONG_KEY];
	char * aux = original;
      
  if(setKey(desKey,password)<0)
    return NULL;
// 	printf("El malloc interno: %d\n",size+LONG_KEY-size%LONG_KEY);
	if((resp=malloc(size+(LONG_KEY-size%LONG_KEY)))==NULL)
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    exit(1);
  }

	encriptar(resp,original,size,desKey);
		
	free(aux);
  return resp;
}
int
setKey(char desKey[LONG_KEY], char * password)
{

  int longPass=0;
  int i=0;
  if(password==NULL)
    return ENCRYPT_ERROR;
  longPass=strlen(password);
  if(longPass < LONG_KEY)
  {
    strcpy(desKey,password);
    for(i=strlen(desKey);i<LONG_KEY;i++)
    {
      desKey[i]=0;
    }
  }
  else if(longPass > LONG_KEY)
  {
    memcpy(desKey,password,LONG_KEY);
  }
  else
  {
//     printf("Password de %d digitos y dice %s\n",strlen(password),password);
		memcpy(desKey,password,LONG_KEY);
// 		printf("En deskey dice %s\n",desKey);
  }
  return 1;
}

int
encriptar(char * resp, char * original,int size, char * desKey)
{
  
  int i=0;
  int j=0;
  int cant;
  char aux[LONG_KEY];
  cant=size+(LONG_KEY-size%LONG_KEY);
  for(i =0 ; i < size-size%LONG_KEY ; i=i+LONG_KEY)
  {
    des_encipher(&original[i],&resp[i], desKey);
	}
  if(size%LONG_KEY!=0)
  {
    memcpy(aux,&original[i],size%LONG_KEY);
    for(j=size% LONG_KEY;j<LONG_KEY; j++)
    {
      aux[j]=0;
    }
    des_encipher(aux,&resp[i],desKey);
  }
  return 1;
}

char*
desencriptarWrapper(char * encriptado,int size,char * password)
{
  char * resp;
  char desKey[LONG_KEY];
  if(setKey(desKey,password)<0)
    return NULL;
  if((resp=malloc(size))==NULL)
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    exit(1);
  }
	desencriptar(resp,encriptado,size,desKey);
  free(encriptado);
  return resp;
}


int
desencriptar(char * resp, char * encriptado,int size, char * desKey)
{
  char * buffer;
  int i=0;
  int cant;
  int sizeEncriptado= size + ((size%LONG_KEY==0)?0: (LONG_KEY - size % LONG_KEY));
    
  if((buffer=malloc(sizeEncriptado))==NULL)
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    exit(1);
  }
	for(i =0 ; i < sizeEncriptado ; i=i+LONG_KEY)
  {
    des_decipher(&encriptado[i],&buffer[i], desKey);
	}
	memcpy(resp,buffer,size);
  free(buffer);
  return 1;
}


