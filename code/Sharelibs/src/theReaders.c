#include "../include/theReaders.h"

/*Funcion de Lectura para la opcion de ConUsr*/

/*{CON_USR,PRE_BUY_MOVIE,BUY_MOVIE,PRE_CHG_PASS,CHG_PASS,DOWNINFO,DOWNLOAD,DOWNDATA,PRE_LIST_CLIENT,LIST_CLIENT,LIST_GEN,LIST_FILMS,DISCONECT,HELP,EXIT};*/
void*
readConUsr(int fd,int block,int * mcookie,char * psw)
{
  conUserT* resp;
  char * buffer2;

  if((resp=malloc(sizeof(conUserT)))==NULL)
  {
    fprintf(stderr,"Error Fatal de Memoria.\n");
    exit(1);
  }
   if((buffer2=malloc(CON_MSG+sizeof(int)))==NULL)
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    exit(1);
  }
  if(readFromSock(fd,(CON_MSG+sizeof(int)),block,buffer2)==READ_ERROR)
  {
		free(resp);
		free(buffer2);
        printf("salgo por aca\n");
		return NULL;
  }
  memcpy(resp->data,buffer2,CON_MSG);
  *mcookie=ntohl(*((int*)(&(buffer2[CON_MSG]))));
  free(buffer2);
  
  return resp;
}

/*Funcion de Lectura para la opcion de PreBuyMovie*/

void*
readPreBuyMovie(int fd,int block,int * mcookie, char * psw)
{

	char * buffer;
	
	if((buffer=malloc(sizeof(int)))==NULL)
    {
      fprintf(stderr,"Error fatal de memoria.\n");
      exit(1);
    }

	readFromSock(fd,(sizeof(int)),block,buffer);
	
	if(buffer==NULL){
		free(buffer);
		return NULL;
	}

	*mcookie=ntohl(*((int*)buffer));
	free(buffer);
	return NULL;
}

void*
readBuyMovie(int fd,int block,int * mcookie,char * psw)
{
  buyMovieT* resp;
  char * buffer2;
  char * desencriptado;
  int newAccountNumber=0;
  int newTicket=0;
  int size= BUY_MOVIE_MSG+sizeof(int);
  size = size + LONG_KEY-size%LONG_KEY;
  int newInt=0;
  
  if((resp=malloc(sizeof(buyMovieT)))==NULL)
  {
    fprintf(stderr,"Error Fatal de Memoria.\n");
    exit(1);
  }
  if((buffer2=malloc(size))==NULL)
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    exit(1);
  }
  readFromSock(fd,size,block,buffer2);
  if(buffer2 == NULL)
  {
    free(resp);
	free(buffer2);
	return NULL;
  }

  desencriptado= desencriptarWrapper(buffer2,(BUY_MOVIE_MSG + sizeof(int)),psw);
  
  memcpy(resp->movie,desencriptado,MAX_NAME+1);
  memcpy(resp->pServer,&(desencriptado[MAX_NAME+1]),MAX_NAME+1);
  newAccountNumber=ntohl(*((int*)(&(desencriptado[2*(MAX_NAME+1)]))));
  newTicket=ntohl(*((int*)(&(desencriptado[2*(MAX_NAME+1)+sizeof(int)]))));
  
  resp->accountNumber=newAccountNumber;
  resp->ticket=newTicket;
  *mcookie=ntohl(*((int*)(&(desencriptado[BUY_MOVIE_MSG]))));
  
  free(desencriptado);
  
  return resp;
}

/*Funcion de Lectura para la opcion de preChgPass*/

void*
readPreChg(int fd,int block,int *mcookie,char * psw)
{

  char * buffer;
    
  if( (buffer=malloc(sizeof(int))) == NULL )
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    exit(1);
  }

  readFromSock(fd,(sizeof(int)),block,buffer);
  
  if(buffer == NULL)
    return NULL;

  *mcookie=ntohl(*((int*)buffer));
  free(buffer);
  return NULL;

}

/*Funcion de Lectura para la opcion de ChgPass*/

void*
readChg(int fd,int block,int *mcookie,char * psw)
{

  chgPassT* resp;
  char * buffer2;
  char * desencriptado;
  int eltamanio = CHG_PASS_MSG+sizeof(int);
	eltamanio = eltamanio + LONG_KEY-eltamanio%LONG_KEY;
  int newInt=0;
  
  if((resp=malloc(sizeof(chgPassT)))==NULL)
  {
    fprintf(stderr,"Error Fatal de Memoria.\n");
    exit(1);
  }
  if((buffer2=malloc(eltamanio))==NULL)
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    free(resp);
    exit(1);
  }
  readFromSock(fd,eltamanio,block,buffer2);
  if(buffer2 == NULL)
  {
    free(resp);
    free(buffer2);
    return NULL;
  }

  desencriptado = desencriptarWrapper(buffer2,eltamanio,psw);
  memcpy( resp->oldPass,&desencriptado[0],MAX_PASS);
  memcpy( resp->newPass,&desencriptado[MAX_PASS],MAX_PASS);


  newInt = ntohl( *((int*)(&(desencriptado[MAX_PASS * 2]))) );
  resp->resp = newInt;
  *mcookie = ntohl( *((int*)(&(desencriptado[CHG_PASS_MSG]))) );
  
  free(desencriptado);
  
  return resp;

}


void*
readDownData(int fd,int block,int * mcookie, char * psw)
{

  DownDataT* resp;
  char * buffer2;

  int size= DOWNDATA_MSG+sizeof(int);

  if((resp=malloc(sizeof(DownDataT)))==NULL)
  {
    fprintf(stderr,"Error Fatal de Memoria.\n");
    exit(1);
  }
   if((buffer2=malloc(size))==NULL)
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    exit(1);
  }
	
  readFromSock(fd,size,block,buffer2);

	if(buffer2==NULL)
  {
		free(resp);
		free(buffer2);
		return NULL;
  }

  resp->longPart=ntohl(*((int*)(&(buffer2[0]))));
  memcpy(resp->part,&buffer2[sizeof(int)],LONG_PACKAGE);
  *mcookie=ntohl(*((int*)(&buffer2[DOWNDATA_MSG])));
  free(buffer2);
  return resp;
}


void*
readDownInfo(int fd,int block,int * mcookie, char * psw)
{

  DownInfoT* resp;

  char * buffer2;
  int size= DOWNINFO_MSG+sizeof(int);

  if((resp=malloc(sizeof(DownInfoT)))==NULL)
  {
    fprintf(stderr,"Error Fatal de Memoria.\n");
    exit(1);
  }

  if((buffer2=malloc(size))==NULL)
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    exit(1);
  }
  readFromSock(fd,size,block,buffer2);
  if(buffer2==NULL)
  {
		free(resp);
		free(buffer2);
		return NULL;
	}
	
  resp->ticket = ntohl( *((int*)(&(buffer2[0]))) );
  resp->codError = ntohl( *((int*)(&(buffer2[sizeof(int)]))) );
  resp->left = ntohl( *((int*)(&(buffer2[2*sizeof(int)]))) );
  memcpy( (resp->extension),&(buffer2[3*sizeof(int)]),MAX_EXT );
  memcpy( (resp->name),&(buffer2[3*sizeof(int)+MAX_EXT]),MAX_NAME );
  *mcookie = ntohl( *((int*)(&(buffer2[DOWNINFO_MSG]))) );
  free(buffer2);

  return resp;
}

/*Funcion de Lectura para la opcion de ListGen*/

void* 
readListGen(int fd,int block,int * mcookie, char * psw)
{

	ListGenT* resp;

  char * buffer;
  char * buffer2;

  if( (resp=malloc(sizeof(ListGenT)))==NULL )
  {
    fprintf(stderr,"Error Fatal de Memoria.\n");
    exit(1);
  }

	/*no cuento el size de la magic cookie, porque lo levanto despues de levantar
		MAX_GENDER, dependiendo del valor de cantgen*/
	int firstSize = sizeof(int);

	int size = sizeof(int)+MAX_GENDER;

  if( (buffer2=malloc(firstSize))==NULL )
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    exit(1);
  }

  readFromSock(fd,firstSize,block,buffer2);

  if( buffer2 == NULL )
  {
    free(resp);
    free(buffer2);
    return NULL;
  }

  resp->cantGen = ntohl( *((int*)(&(buffer2[0]))) ) ;

	if(resp->cantGen == 0)
	{
		readFromSock(fd,firstSize,block,buffer2);

		*mcookie = ntohl(*((int*)(&buffer2[0])));
		free(buffer2);
		return resp;
	}

	free(buffer2);

	if( (buffer2=malloc(size))==NULL )
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    exit(1);
  }

	readFromSock(fd,size,block,buffer2);
	memcpy(resp->gen, &(buffer2[0]), MAX_GENDER);

	*mcookie = ntohl(*((int*)(&buffer2[MAX_GENDER])));
	free(buffer2);

  return resp;
}

/*Funcion de Lectura para la opcion de preListClient*/

void* 
readPreListClient(int fd,int block,int * mcookie, char * psw)
{
  char * buffer;
    
  if( (buffer=malloc(sizeof(int))) == NULL )
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    exit(1);
  }

  readFromSock(fd,(sizeof(int)),block,buffer);
  
  if(buffer == NULL)
    return NULL;

  *mcookie=ntohl(*((int*)buffer));
  free(buffer);
  return NULL;

}



/*Funcion de Lectura para la opcion de ListMovies*/

void* 
readListFilm(int fd,int block,int * mcookie, char * psw)
{
	
  ListMovieT_Send* resp;

  char * buffer;
  char * buffer2;

  if( (resp=malloc(sizeof(ListMovieT_Send)))==NULL )
  {
    fprintf(stderr,"Error Fatal de Memoria.\n");
    exit(1);
  }

	/*no cuento el size de la magic cookie, porque lo levanto despues de levantar
		la despripcion variable*/
  int size = 3*sizeof(int)+sizeof(double)+MAX_NAME+MAX_GENDER+MAX_MD5;
	int firstSize = sizeof(int);

  if( (buffer2=malloc(firstSize))==NULL )
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    exit(1);
  }
  
  readFromSock(fd,firstSize,block,buffer2);

  if( buffer2 == NULL )
  {
    free(resp);
    free(buffer2);
    return NULL;
  }
  
  resp->cantFilm = ntohl( *((int*)(&(buffer2[0]))) ) ;
	
	if(resp->cantFilm == 0)
	{
		readFromSock(fd,firstSize,block,buffer2);
		*mcookie = ntohl(*((int*)(&buffer2[0])));
		free(buffer2); 
		return resp;
	}

	if(resp->cantFilm == -1)
	{
		free(buffer2);
		
		int inSize = sizeof(int)+MAX_NAME+MAX_GENDER;
		
		if( (buffer2=malloc(inSize))==NULL )
  	{
    	fprintf(stderr,"Error fatal de memoria.\n");
    	exit(1);
  	}		
		
		readFromSock(fd,inSize,block,buffer2);
		
		memcpy(resp->movie.name, &(buffer2[0]), MAX_NAME);
  	memcpy(resp->movie.genero, &(buffer2[MAX_NAME]), MAX_GENDER);
		*mcookie = ntohl(*((int*)(&buffer2[MAX_NAME+MAX_GENDER])));
		free(buffer2); 
		return resp;
	}


	free(buffer2);

	if( (buffer2=malloc(size))==NULL )
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    exit(1);
  }

	readFromSock(fd,size,block,buffer2);

	memcpy(resp->movie.name, &(buffer2[0]), MAX_NAME);
  memcpy(resp->movie.genero, &(buffer2[MAX_NAME]), MAX_GENDER);
  resp->movie.minutos = ntohl( *((int*)&(buffer2[MAX_NAME+MAX_GENDER])) );
  resp->movie.megas = ntohl( *((int*)&(buffer2[1*sizeof(int)+MAX_NAME+MAX_GENDER])) );
	resp->movie.precio = ntohl( *((double*)&(buffer2[2*sizeof(int)+MAX_NAME+MAX_GENDER])) );
	memcpy(resp->movie.mdFive, &(buffer2[2*sizeof(int)+sizeof(double)+MAX_NAME+MAX_GENDER]), MAX_MD5);
	resp->movie.longDescription = ntohl(*((int*)(&(buffer2[2*sizeof(int)+sizeof(double)+MAX_NAME+MAX_GENDER+MAX_MD5]))) );
	    
  free(buffer2);

	/*la cadena description + la magic cookie*/
  if( (buffer2=malloc(resp->movie.longDescription + sizeof(int))) == NULL )
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    free(buffer);
    free(resp);
    exit(1);
  }

  readFromSock(fd,resp->movie.longDescription + sizeof(int),block,buffer2);
  
  if( buffer2 == NULL )
  {
    free(resp);
    free(buffer2);
    return NULL;
  }

	if( (resp->movie.description = malloc(resp->movie.longDescription)) == NULL )
	{
		fprintf(stderr,"Error fatal de memoria.\n");
		free(buffer2);
		free(resp);
		return NULL;
	}

  strcpy(resp->movie.description, buffer2);
  *mcookie = ntohl(*((int*)(&buffer2[resp->movie.longDescription])));  
  free(buffer2);

  return resp;
}

/*Funcion de Lectura para la opcion de ReadDisco*/

void* 
readDisco(int fd,int block,int * mcookie, char * psw)
{

	DisconnectT* resp;
  char * buffer2;
	int size= DISCONNECT_MSG+sizeof(int);

  if( (resp=malloc(sizeof(DisconnectT))) == NULL)
  {
    fprintf(stderr,"Error Fatal de Memoria.\n");
    exit(1);
  }
   
	if((buffer2=malloc(size))==NULL)
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    exit(1);
  }

  readFromSock(fd,size,block,buffer2);
  
	if(buffer2==NULL)
  {
		free(resp);
		free(buffer2);
		return NULL;
  }
  
	resp->cod = ntohl(*((int*)(&(buffer2[0]))));
  *mcookie=ntohl(*((int*)(&buffer2[DISCONNECT_MSG])));

  free(buffer2);
  return resp;

}

/*Funcion de Lectura para la opcion de ListClient*/

void*
readListClient(int fd,int block,int * mcookie, char * psw)
{

	ListClientT_Send* resp;

  char * buffer;
  char * buffer2;
  char * desencriptado;

  if( (resp=malloc(sizeof(ListClientT_Send)))==NULL )
  {
    fprintf(stderr,"Error Fatal de Memoria. 44\n");
    exit(1);
  }

  int size = 2*sizeof(int)+MAX_NAME+MAX_MAIL;

  if( (buffer2=malloc(2*sizeof(int))) == NULL )
  {
    fprintf(stderr,"Error fatal de memoria. 33\n");
    exit(1);
  }

	/*En esta primer lectura leo clientCant*/

  readFromSock(fd,2*sizeof(int),block,buffer2);

  if( buffer2 == NULL )
  {
    free(resp);
    return NULL;
  }

/*SI EL CLIENTE ES 0 TENGO Q LEVANTAR LA MAGIC COOKIE NADA MAS Y SALIR*/
  desencriptado = desencriptarWrapper(buffer2,2*sizeof(int),psw);
  resp->clientCant = ntohl( *((int*)(&(desencriptado[0]))) ) ;
		


	if(resp->clientCant == 0)
	{
		if( (buffer2=malloc(2*sizeof(int))) == NULL )
  	{
  	  fprintf(stderr,"Error fatal de memoria. 33\n");
  	  exit(1);
  	}
		readFromSock(fd,2*sizeof(int),block,buffer2);
		desencriptado = desencriptarWrapper(buffer2,2*sizeof(int),psw);
		*mcookie = ntohl( *((int*)(&(desencriptado[0]))) );
		free(desencriptado);
		return resp;
	}

	free(desencriptado);	
	
	if( ((buffer2=malloc(resp->clientCant)))==NULL )
  {
    fprintf(stderr,"Error fatal de memoria 22.\n");
    exit(1);
  }

	readFromSock(fd,resp->clientCant,block,buffer2);
	desencriptado = desencriptarWrapper(buffer2,resp->clientCant,psw);

	memcpy(resp->cliente.name, &(desencriptado[0]), MAX_NAME);

  memcpy(resp->cliente.mail, &(desencriptado[MAX_NAME]), MAX_MAIL);

	resp->cliente.level = ntohl( *((int*)(&(desencriptado[MAX_NAME+MAX_MAIL]))) );

	resp->cliente.longDescription = ntohl( *((int*)(&(desencriptado[sizeof(int)+MAX_NAME+MAX_MAIL]))) );

  
  
	if( (resp->cliente.description=malloc(resp->cliente.longDescription)) == NULL )
  {
    fprintf(stderr,"Error fatal de memoria 11.\n");
    free(resp);
		free(desencriptado);
    exit(1);
  }



  strcpy(resp->cliente.description, &(desencriptado[2*sizeof(int)+MAX_NAME+MAX_MAIL]));

	*mcookie = ntohl(*((int*)(&desencriptado[2*sizeof(int)+MAX_NAME+MAX_MAIL+resp->cliente.longDescription])));


	

	free(desencriptado);

  return resp;

}




