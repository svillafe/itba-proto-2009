#include "../include/parse.h"

char*
parseaPreBuyMovie(TransportT* msg,int*size)
{
	char *resp;
	int magicCookie;
	
	magicCookie = htonl(msg->magicCookie);
	*size = sizeof(int);
	
	if((resp=malloc(sizeof(int)))==NULL)
  {
    fprintf(stderr,"Error de memoria, falta memoria");
    exit(1);
  }

	memcpy(resp,&magicCookie,sizeof(int));

	return resp;
}

char*
parseaBuyMovie(TransportT* msg,int*size)
{
  char * resp;
  int goTicket;
  int goAccountNumber;
  int magicCookie;
  buyMovieT* aux;
  
  magicCookie= htonl(msg->magicCookie);
  *size=BUY_MOVIE_MSG+sizeof(int);
  aux = (buyMovieT*)((msg->data.opData));
  goTicket=htonl(aux->ticket);
  goAccountNumber = htonl(aux->accountNumber);

  if((resp=malloc(BUY_MOVIE_MSG+sizeof(int)))==NULL)
  {
    fprintf(stderr,"Error de memoria, falta memoria");
    exit(1);
  }

  memcpy(resp,aux->movie,MAX_NAME+1);
  memcpy(&resp[MAX_NAME+1],aux->pServer,MAX_NAME+1);
  memcpy(&resp[2*(MAX_NAME+1)],&goAccountNumber,sizeof(int));
  memcpy(&resp[2*(MAX_NAME+1)+sizeof(int)],&goTicket,sizeof(int));
  memcpy(&resp[BUY_MOVIE_MSG],&magicCookie,sizeof(int));
  
  return resp;
}


char*
parseaCon(TransportT* msg,int*size)
{
  char* resp=NULL;
  int magicCookie;
  conUserT* aux; 
  
  magicCookie= htonl(msg->magicCookie);
  *size=CON_MSG+sizeof(int);
  aux = (conUserT*)((msg->data.opData));
  
  if((resp=malloc(CON_MSG+sizeof(int)))==NULL)
  {
    fprintf(stderr,"Error de memoria, falta memoria");
    exit(1);
  }

  memcpy(resp,aux->data,CON_MSG);/*Agrego al buffer todos los elementos de conUsrT*/
  memcpy(&resp[CON_MSG],&magicCookie,sizeof(int));
  
  return resp;
}

char*
parseaPreChg(TransportT * msg,int * size)
{
	char *resp;
	int magicCookie;
	
	magicCookie = htonl(msg->magicCookie);
	*size = sizeof(int);
	
	if((resp=malloc(sizeof(int)))==NULL)
  {
    fprintf(stderr,"Error de memoria, falta memoria");
    exit(1);
  }

	memcpy(resp,&magicCookie,sizeof(int));

	return resp;
}


char*
parseaChg(TransportT * msg, int * size)
{


  char * resp;
  int msgResp;
  int magicCookie;
  chgPassT* aux;
  
  magicCookie= htonl(msg->magicCookie);
  int eltamanio = CHG_PASS_MSG+sizeof(int);
	eltamanio = eltamanio + LONG_KEY-eltamanio%LONG_KEY;	


	*size= eltamanio;
  aux = (chgPassT*)((msg->data.opData));
  msgResp=htonl(aux->resp);
  
  if((resp=malloc(eltamanio))==NULL)
  {
    fprintf(stderr,"Error de memoria, falta memoria");
    exit(1);
  }

  memcpy(resp,aux->oldPass,MAX_PASS);
  memcpy(&resp[MAX_PASS],aux->newPass,MAX_PASS);
  memcpy(&resp[MAX_PASS * 2],&msgResp,sizeof(int));
  memcpy(&resp[CHG_PASS_MSG],&magicCookie,sizeof(int));
  

  return resp;

}


char *
parseaDownInfo(TransportT * msg,int * size)
{
  char* resp;
  int magicCookie;
	int goTicket;
	int goCodError;
	int goLeft;
	DownInfoT * aux;

	magicCookie= htonl(msg->magicCookie);
	*size=DOWNINFO_MSG+sizeof(int);
  aux = (DownInfoT*)((msg->data.opData));
  goTicket = htonl(aux->ticket);
	goCodError = htonl(aux->codError);
	goLeft = htonl(aux->left);

	if((resp=malloc(DOWNINFO_MSG+sizeof(int)))==NULL)
  {
    fprintf(stderr,"Error de memoria, falta memoria");
    exit(1);
  }

  memcpy(&resp[0],&goTicket,sizeof(int));
  memcpy(&resp[sizeof(int)],&goCodError,sizeof(int));
  memcpy(&resp[(2*sizeof(int))],&goLeft,sizeof(int));
  memcpy(&resp[(3*sizeof(int))],aux->extension,MAX_EXT);
  memcpy(&resp[(3*sizeof(int))+MAX_EXT],aux->name,MAX_NAME);
	memcpy(&resp[DOWNINFO_MSG],&magicCookie,sizeof(int));
  return resp;

}

char*
parseaDownData(TransportT * msg,int * size)
{
  char* resp;
  int magicCookie= htonl(msg->magicCookie);
	int goLastPart;
  *size=DOWNDATA_MSG+sizeof(int);
  DownDataT * aux = (DownDataT *)((msg->data.opData));
  goLastPart = htonl(aux->longPart);
	if((resp=malloc(DOWNDATA_MSG+sizeof(int)))==NULL)
  {
    fprintf(stderr,"Error de memoria, falta memoria");
    exit(1);
  }
  memcpy(&resp[0],&goLastPart,sizeof(int));
  memcpy(&resp[sizeof(int)],&(aux->part),LONG_PACKAGE);
  memcpy(&resp[DOWNDATA_MSG],&magicCookie,sizeof(int));
  return resp;
} 

char*
parseaListGen(TransportT * msg,int * size)
{

	char* resp;
  int magicCookie= htonl(msg->magicCookie);
  int genVariable;

  ListGenT * aux = (ListGenT*)((msg->data).opData);
  genVariable = aux->cantGen;

  
	/*Le mando el ultimo genero, en realidad sin ningun genero*/
  if(genVariable == 0)
  {
    /*por el cantFilm y la magic Cookie*/
    if( (resp = malloc(2*sizeof(int))) == NULL )
    {
      fprintf(stderr,"\nError fatal en el malloc\n");
      exit(1);
    }
    *size = 2*sizeof(int);
    genVariable = htonl(genVariable);
    memcpy(&resp[0],&genVariable,sizeof(int));
    memcpy(&resp[sizeof(int)],&magicCookie,sizeof(int));

		return resp;

  }

	/*de la magic cookie y el genero*/
  int tamanio = 2*sizeof(int)+MAX_GENDER;
  /*int tamanio = 2*sizeof(int)+MAX_GENDER+MAX_DESC_GEN;*/
	*size = tamanio;

  if( (resp = malloc(tamanio)) == NULL )
  {
    fprintf(stderr,"\nError fatal en el malloc\n");
    exit(1);
  }


  genVariable = htonl(genVariable);
	memcpy(&resp[0],&genVariable, sizeof(int));
	memcpy(&resp[sizeof(int)],aux->gen, MAX_GENDER);
	/*
		memcpy(&resp[sizeof(int)+MAX_GENDER],aux->descGen, MAX_DESC_GEN);
		memcpy(&resp[MAX_GENDER+sizeof(int)+MAX_DESC_GEN],&magicCookie,sizeof(int));
	*/
  memcpy(&resp[MAX_GENDER+sizeof(int)],&magicCookie,sizeof(int));

  return resp;
}

char*
parseaListMovies(TransportT * msg,int * size)
{

	char* resp;
  int magicCookie= htonl(msg->magicCookie);
  int filmVariable;


  
  ListMovieT_Send * aux = (ListMovieT_Send *)((msg->data).opData);

  filmVariable = aux->cantFilm;

  if((filmVariable != 1) && (filmVariable != 0) && (filmVariable != -1)  )
  {
    printf("Error en los parametros de la estructura list cliente\n");
    return NULL;
  }



	/*Le mando la última pelicula, en realidad sin pelicula*/
  if(filmVariable == 0)
  {
    /*por el cantFilm y la magic Cookie*/
    if( (resp = malloc(2*sizeof(int))) == NULL )
    {
      fprintf(stderr,"\nError fatal en el malloc\n");
      exit(1);
    }
    *size = 2*sizeof(int);
    filmVariable = htonl(filmVariable);
    memcpy(&resp[0],&filmVariable,sizeof(int));
    memcpy(&resp[sizeof(int)],&magicCookie,sizeof(int));
 
		return resp;

  }  

	if(filmVariable == -1)
  {
    int inSize = 2*sizeof(int) +MAX_NAME +MAX_GENDER;
		/*por el cantFilm y la magic Cookie*/
    if( (resp = malloc(inSize)) == NULL )
    {
      fprintf(stderr,"\nError fatal en el malloc\n");
      exit(1);
    }
    *size = inSize;
    filmVariable = htonl(filmVariable);
    memcpy(&resp[0],&filmVariable,sizeof(int));
		memcpy(&resp[sizeof(int)],(aux->movie).name,MAX_NAME);
		memcpy(&resp[sizeof(int)+MAX_NAME],(aux->movie).genero,MAX_GENDER);
    memcpy(&resp[sizeof(int)+MAX_NAME+MAX_GENDER],&magicCookie,sizeof(int));
 
		return resp;

  }

	

	int minutos = aux->movie.minutos;
	int megas = aux->movie.megas;
	int descripcion = aux->movie.longDescription;
	int descripcion2 = descripcion;
	double precio = aux->movie.precio;
  
  int tamanio = 5*sizeof(int)+sizeof(double)+MAX_NAME+MAX_GENDER+MAX_MD5+descripcion;
  *size = tamanio;

  if( (resp = malloc(tamanio)) == NULL )
  {
    fprintf(stderr,"\nError fatal en el malloc\n");
    exit(1);    
  } 
  

  filmVariable = htonl(filmVariable);
  descripcion = htonl(descripcion);
  megas = htonl(megas);  
  minutos = htonl(minutos);
	precio = htonl(precio);

  memcpy(&resp[0],&filmVariable,sizeof(int));
  memcpy(&resp[sizeof(int)],aux->movie.name, MAX_NAME);
  memcpy(&resp[sizeof(int)+MAX_NAME],aux->movie.genero, MAX_GENDER);
  memcpy(&resp[sizeof(int)+MAX_NAME+MAX_GENDER],&minutos,sizeof(int));
  memcpy(&resp[2*sizeof(int)+MAX_NAME+MAX_GENDER],&megas,sizeof(int));
  memcpy(&resp[3*sizeof(int)+MAX_NAME+MAX_GENDER],&precio,sizeof(double));
	memcpy(&resp[3*sizeof(int)+sizeof(double)+MAX_NAME+MAX_GENDER],aux->movie.mdFive,MAX_MD5);
	
	memcpy(&resp[3*sizeof(int)+sizeof(double)+MAX_NAME+MAX_GENDER+MAX_MD5],&descripcion,sizeof(int));
	strcpy(&resp[4*sizeof(int)+sizeof(double)+MAX_NAME+MAX_GENDER+MAX_MD5],aux->movie.description);
  memcpy(&resp[4*sizeof(int)+sizeof(double)+MAX_NAME+MAX_GENDER+MAX_MD5+descripcion2],&magicCookie,sizeof(int));
	


  return resp;
}

char*
parseaDisco(TransportT * msg,int * size)
{

	char* resp;
  int magicCookie= htonl(msg->magicCookie);
	*size=DISCONNECT_MSG+sizeof(int);
  DisconnectT * aux = (DisconnectT *)((msg->data.opData));
	int cod;
	cod = htonl(aux->cod);

	if( (resp = malloc(*size)) == NULL )
  {
    fprintf(stderr,"Error de memoria, falta memoria");
    exit(1);
  }
  memcpy(&resp[0],&cod,sizeof(int));
  memcpy(&resp[DISCONNECT_MSG],&magicCookie,sizeof(int));

  return resp;


}

char *
parseaListClient(TransportT * msg,int * size)
{

  char* resp;
  int magicCookie= htonl(msg->magicCookie);
  int descVariable;
  int clientsVariable;


  ListClientT_Send * aux = (ListClientT_Send *)((msg->data).opData);
 
  clientsVariable = aux->clientCant;
  
	if((clientsVariable != 1) && (clientsVariable != 0))
  {
    printf("Error en los parametros de la estructura list cliente\n");
    return NULL;
  }

  if(clientsVariable == 0)
  {
    /*por el clientCant y la magic Cookie*/
    if( (resp = malloc(4*sizeof(int))) == NULL )
    {
      fprintf(stderr,"\nError fatal en el malloc\n");
      exit(1);
    }
    *size = 4*sizeof(int);
    clientsVariable = htonl(clientsVariable);
    memcpy(&resp[0],&clientsVariable,sizeof(int));
		memcpy(&resp[sizeof(int)],&clientsVariable,sizeof(int));
    memcpy(&resp[2*sizeof(int)],&magicCookie,sizeof(int));
    memcpy(&resp[3*sizeof(int)],&magicCookie,sizeof(int));
		return resp;

  }  
  

	
  int descripcion = aux->cliente.longDescription;
  int descripcion2 = descripcion;
  int level = aux->cliente.level;

	int eltamanio = 2*sizeof(int) + MAX_NAME+MAX_MAIL+descripcion+3*sizeof(int);
	eltamanio = eltamanio + LONG_KEY-eltamanio%LONG_KEY;
	int tamanio = 3*sizeof(int)+MAX_NAME+MAX_MAIL+descripcion;
	tamanio = tamanio + LONG_KEY-tamanio%LONG_KEY;
		
	*size = eltamanio;

  if( (resp = malloc(eltamanio)) == NULL )
  {
    fprintf(stderr,"\nError fatal en el malloc\n");
    exit(1);    
  } 
  
  clientsVariable = htonl(tamanio);
  descripcion = htonl(descripcion);
  level = htonl(level);

  memcpy(&resp[0],&clientsVariable,sizeof(int));
  memcpy(&resp[sizeof(int)],&clientsVariable,sizeof(int));
	memcpy(&resp[2*sizeof(int)],aux->cliente.name, MAX_NAME);
  memcpy(&resp[2*sizeof(int)+MAX_NAME],aux->cliente.mail, MAX_MAIL);
  memcpy(&resp[2*sizeof(int)+MAX_NAME+MAX_MAIL],&level,sizeof(int));
  memcpy(&resp[3*sizeof(int)+MAX_NAME+MAX_MAIL],&descripcion,sizeof(int));
  strcpy(&resp[4*sizeof(int)+MAX_NAME+MAX_MAIL],aux->cliente.description);
  memcpy(&resp[4*sizeof(int)+MAX_NAME+MAX_MAIL+descripcion2],&magicCookie,sizeof(int));

  return resp;

}

char *
parseaPreListClient(TransportT * msg,int * size)
{
	char *resp;
	int magicCookie;
	
	magicCookie = htonl(msg->magicCookie);
	*size = sizeof(int);
	
	if((resp=malloc(sizeof(int)))==NULL)
    {
      fprintf(stderr,"Error de memoria, falta memoria");
      exit(1);
    }

	memcpy(resp,&magicCookie,sizeof(int));

	return resp;
}






