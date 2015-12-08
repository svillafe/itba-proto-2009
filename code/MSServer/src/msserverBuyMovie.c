#include "../include/msserverBuyMovie.h"

extern endPoint infoPl;

int 
buyMovie(comunicationT * mensaje, int fd)
{
	/*char * pass= "lalalala";*/
	char pass[MAX_PASS];
	comunicationT dataMsg;
	buyMovieT * entryData;
	char password[MAX_PASS];
  char movieName[MAX_NAME];
	comunicationT rta;
	int error;
	rta.opcode=BUY_MOVIE;
	int nivel;
  memcpy(rta.name,mensaje->name,MAX_NAME);
  if((rta.opData=malloc(sizeof(buyMovieT)))==NULL)
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    exit(1);
  }

	if (getUserPswd(mensaje->name,pass))
	{
		fprintf(stderr,"No se pudo obtener la clave del usuario %s",mensaje->name);
		exit(1);
	}

// 	printf("pass %s usr %s \n\n",pass, mensaje->name);

  if(!initSerCon(mensaje->name))
  {
  	fprintf(stderr,"No se pudo validar la identidad del usuario %s. Se cierra proceso.",mensaje->name);
  	*(((buyMovieT*)(rta.opData))->movie) = 'E';
  	((buyMovieT*)(rta.opData))->ticket = NOT_FOUND_MOVIE_TICKET;
  	if( enterMsgCom(&rta,fd,NO_BLOCK,pass) < 1)
		{
			fprintf(stderr,"No se pudo enviar la información\n");
			free(rta.opData);
			exit(1);
		}
		free(rta.opData);
  	exit(1);
  }
	
	if(getUserLevel(mensaje->name,&nivel)!=0)
	{
		fprintf(stderr,"Error al obtener el nivel del LDAP\n");
		*(((buyMovieT*)(rta.opData))->movie) = 'E';
		((buyMovieT*)(rta.opData))->ticket = NOT_FOUND_MOVIE_TICKET;
		if( enterMsgCom(&rta,fd,NO_BLOCK,pass) < 1)
		{
			fprintf(stderr,"No se pudo enviar la información\n");
			free(rta.opData);
			realeaseServer(mensaje->name);
			exit(1);
		}
		realeaseServer(mensaje->name);
		free(rta.opData);
		exit(1);
	}
	
	if(!initFileMovie(mensaje->name,nivel))
	{
		fprintf(stderr,"Error al cargar el archivo de FILE.DATA\n");
		*(((buyMovieT*)(rta.opData))->movie) = 'E';
		((buyMovieT*)(rta.opData))->ticket = NOT_FOUND_MOVIE_TICKET;
		if( enterMsgCom(&rta,fd,NO_BLOCK,pass) < 1)
		{
			fprintf(stderr,"No se pudo enviar la información\n");
			free(rta.opData);
			realeaseServer(mensaje->name);
			exit(1);
		}
		realeaseServer(mensaje->name);
		free(rta.opData);
  	exit(1);
	}

// 	printf("Estoy por aca 1\n");

	/*if (getUserPswd(mensaje->name,pass))
		{
			fprintf(stderr,"No se pudo obtener la clave del usuario %s",mensaje->name);
			exit(1);
		}
	*/
	
	if( getMsgCom(&dataMsg,fd,BLOCK,pass) < 1 )
	{
		fprintf(stderr,"No se pudo recibir la información del cliente\n");
		free(rta.opData);
		realeaseServer(mensaje->name);
  	releaseFileMovie(mensaje->name);
		exit(1);
	}

	entryData = (buyMovieT *)(dataMsg.opData);
    
    /*printf("Recibi: peli:%s, pserver: %s, account number: %d, ticket: %d\n",entryData->movie,
	entryData->pServer,entryData->accountNumber,entryData->ticket);
  */

	strcpy(movieName, (entryData->movie));
//     printf("Estoy por aca 2\n");
	/*Controlar que el usuario no haya comprado previamente la pelicula*/
	if(existeLaPeli(movieName))
	{
		*(((buyMovieT*)(rta.opData))->movie) = 'R';
		((buyMovieT*)(rta.opData))->ticket = NOT_FOUND_MOVIE_TICKET;
	}
    else if(filmExist(movieName))
	{ 
// 	 printf("Estoy por aca 3\n");
// 	 printf("psServer: %s, precio %g, nombre %s, numeroCuenta: %d",entryData->pServer,giveMePrice(movieName),mensaje->name,entryData->accountNumber);
  	 int cod=moneyInAccount(entryData->pServer,giveMePrice(movieName),mensaje->name,entryData->accountNumber);
  	if(cod==-1)
  	{
  		*(((buyMovieT*)(rta.opData))->movie) = 'E';/*Request Mal Formado*/
  		((buyMovieT*)(rta.opData))->ticket = NOT_FOUND_MOVIE_TICKET;
  	}
    else if(cod==1)
    {
    	*(((buyMovieT*)(rta.opData))->movie) = 'F';/*Request Mal Formado*/
      ((buyMovieT*)(rta.opData))->ticket = NOT_FOUND_MOVIE_TICKET;
    } else if(cod==2)
      {
        *(((buyMovieT*)(rta.opData))->movie) = 'A';/*Cuenta no existente*/
        ((buyMovieT*)(rta.opData))->ticket = NOT_FOUND_MOVIE_TICKET;
      } else if(cod==3)
      	{
        	*(((buyMovieT*)(rta.opData))->movie) = '$';/*Sin Plata*/
          ((buyMovieT*)(rta.opData))->ticket = NOT_FOUND_MOVIE_TICKET;
      	}	else
		    { 
              *(((buyMovieT*)(rta.opData))->movie) = 'Y';
      		  ((buyMovieT*)(rta.opData))->ticket = getNumberTicket();
      		  
      		  agregarPelicula(movieName,((buyMovieT*)(rta.opData))->ticket);
      		}
	}
  else
  {
  	*(((buyMovieT*)(rta.opData))->movie) = 'N';
    ((buyMovieT*)(rta.opData))->ticket = NOT_FOUND_MOVIE_TICKET;
  }
	
	enterMsgCom(&rta,fd,NO_BLOCK,pass);

  realeaseServer(mensaje->name);
  releaseFileMovie(mensaje->name);
  exit(1);

}
// 
// 
//       if (getPSReply(&msgRes, pssock)==READ_ERROR)
//           printf("Error al recibir mensaje del payment server\n");
// 
//       printf("\nRespuesta:\n\n");
//       printf("Status code: %d\n",msgRes.statusCode);
//       printf("Valor: %s\n",msgRes.value);


int
connectToPServer(endPoint*ps,psMsgRequestT * msg,psMsgReplyT * resp)
{
  int pssock;
  //con la informacion que me dio el plserver me conecto al pserver
  if (connectToPS(ps,&pssock)==0)
  {
      enterPSRequest(msg,pssock);
      
      if (getPSReply(resp, pssock)==READ_ERROR)
      {
          fprintf(stderr,"Error al recibir mensaje del payment server\n");
          exit(1);
      }
      close(pssock);
      return 1;
  }
  return -1;
}

int 
connecToPlserver(char * pServer,endPoint * ps)
{
  int plsock;
  if(getPSEndpoint(ps,pServer))
  {
    close(plsock);
    return 1;
  }
  if(connectPLServer(&infoPl,&plsock)==0)
  {
    plserverMsgT msg;
    
    strcpy(msg.name,pServer);

    clientEnterPLsrvMsgUDP(&msg, plsock, 0);
	

    if (clientGetPLsrvMsgUDP(&msg,plsock)==READ_ERROR )
    {
      fprintf(stderr,"Error al recibir mensaje del plserver\n");
      return -1;
    }

	*ps=msg.ep;
    saveTTL(&msg);
//     printf("Respuesta:\n\n");
//     printf("Nombre: %s\n", msg.name);
//     printf("IP ps: %s\n", msg.ep.dirIP);
//     printf("Puerto ps: %d\n", msg.ep.numbPort);

    close(plsock);
    return 1;
  }
  else
    return -1;
}

int
moneyInAccount(char * pServer, double precio, char * usuario, int accountNumber)
{
  endPoint ps={0};
  psMsgRequestT msg={0};
  psMsgReplyT msgResp={0};
  if(connecToPlserver(pServer,&ps)==-1)
  {
    return -1;
  }
  strcpy(msg.clientServer,"MMS S.A");
  strcpy(msg.accountName,usuario);
  sprintf(msg.accountNumber,"%d",accountNumber);
//   printf("%s------------\n",msg.accountNumber);
  msg.securityCode=2234;
  msg.amount=precio;
//   printf("Recibido %s, %d\n",ps.dirIP,ps.numbPort);
  if(connectToPServer(&ps,&msg,&msgResp)==-1)
  {
    return -1;
  }
  
  return msgResp.statusCode; 
}



