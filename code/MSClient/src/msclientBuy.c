#include "../include/msclientBuy.h"


extern char ipServer[MAX_IP];
extern char portNumber[MAX_PORT];
extern char clientName[MAX_NAME];
extern char pass[MAX_PASS];

int
buyMovie()
{
	char movieName[MAX_LEN];
  char pServer[MAX_LEN];
	int ticket;
	int sock;
	int intentos = 0;
  int accountNumber=0;
	printf("\n\n--Compra de peliculas--\n");
	

  do
	{
		printf("\n\nIngrese el nombre de la pelicula que desea comprar.(Recuerde");
		printf(" no incluir el caracter '*' en la pelicula)\n\npelicula: ");
    intentos ++;
	} while( (!getString(movieName,"la pelicula")) && (intentos < 4) );


	if(intentos == 4)
	{
		printf("\n\n Saliendo de comprar película\n\n");
		return 0;
	}
    intentos =0;
    do
    {
        printf("\n\nIngrese el nombre del servidor de pago donde posee cuenta");
        printf(" no incluir el caracter '*' en el nombre) \n\nServidor de pago: ");
        intentos ++;
    } while( (!getString(pServer,"el servidor de pago")) && (intentos < 4) );
    
    if(intentos == 4)
    {
        printf("\n\n Saliendo de comprar película\n\n");
        return 0;
    }
	
		intentos = 0;

    do
    {
        printf("\n\nIngrese su numero de cuenta");
        printf("\n\nNumero de cuenta: ");
        intentos ++;
    } while(scanf("%d",&accountNumber)!=1 && (intentos < 4) );
    
    if(intentos == 4)
    {
        printf("\n\n Saliendo de comprar película\n\n");
        return 0;
    }
    LIMPIA_BUFFER;

	printf("\n\nConectandose al servidor para comprar %s.....\n\n", movieName);
	
	comunicationT msgMovie, receiveAns, preBuy;
	
  msgMovie.opcode = BUY_MOVIE;
	strcpy(msgMovie.name, clientName);
	if( (msgMovie.opData = malloc(sizeof(buyMovieT))) == NULL)
	{
		fprintf(stderr,"\n\nNo hay memoria para el malloc\n\n");
		return 0;
	}
	
  strcpy(((buyMovieT*)(msgMovie.opData))->movie, movieName);
  strcpy(((buyMovieT*)(msgMovie.opData))->pServer,pServer);
  ((buyMovieT*)(msgMovie.opData))->accountNumber=accountNumber;


	if( (sock = connectTCP(ipServer, portNumber)) < 0 )
	{
		fprintf(stderr,"Error de conexion\n\n");
		free(msgMovie.opData);
		return 0;
	}


	preBuy.opcode = PRE_BUY_MOVIE;
	strcpy(preBuy.name,clientName);
	preBuy.opData = NULL;

	
	/*Aviso que el mensaje que sigue es encriptado*/
	if(enterMsgCom(&preBuy,sock,NO_BLOCK,pass) < 0)
	{
		fprintf(stderr,"Error al enviar un mensaje\n");
		free(msgMovie.opData);
		close(sock);
		return 0;
	}

	if(enterMsgCom(&msgMovie,sock,NO_BLOCK,pass) < 0)
	{
		fprintf(stderr,"Error al enviar un mensaje\n");
		free(msgMovie.opData);
		close(sock);
		return 0;
	}


	/*recibo la respuesta de si la compra estuvo todo bien*/	
	if(getMsgCom(&receiveAns,sock,NO_BLOCK,pass) < 0)
	{
		printf("\n\nError al recibir la informacion del servidor.\n\n");
		free(msgMovie.opData);
		close(sock);
		return 0;
	}


	switch( *(((buyMovieT*)(receiveAns.opData))->movie) )
	{
		case 'R': 	printf("\nYa tiene la pelicula comprada con posibilidades de");
								printf(" descarga\n");
								return 0;
		case '$': 	printf("\nNo hay dinero suficiente para comprar la");
								printf(" pelicula\n");
								return 0;
		case 'N': 	printf("\nNo existe el nombre de la pelicula %s\n",movieName);
								return 0;
		case 'Y':		printf("\nPelicula comprada con exito\n");
								break;
		case 'E':		printf("\nError en el servidor. Intentelo mas tarde\n");
								return 0;
        case 'F':       printf("\nError el servidor no se pudo comunicar correctamente con el pServer(request mal formado).\n");
                                return 0;
        case 'A':       printf("\nError cuenta no existente.\n");
                                return 0;
		default:		fprintf(stderr,"\n\nRecepcion de algo inesperado.\n\n");
								return 0;
	}


	/*recibo el ticket*/

	ticket = (((buyMovieT*)(receiveAns.opData))->ticket);
	printf("Compraste la pelicula %s !!\n\n",movieName);
	printf("\n\nEl ticket que recibiste por la compra de %s",movieName);
	printf(" fue %d\n\n", ticket);

    close(sock);
	free(msgMovie.opData);

	return 1;
}

int getString(char *resp, char * levanto)
{
	int i;
	
	if(fgets(resp,MAX_LEN-1,stdin) != NULL)
	{
		for(i=0; i<strlen(resp); i++)
    {	
			if(resp[i] == '*')
			{
				printf("\nNo pongas asteriscos en %s!!\n",levanto);
				return 0;
			}
			if(resp[i] == '\n')
				resp[i] = '\0';
		}
		return 1;
	}		
		
	fprintf(stderr,"\n\nError de lectura de %s\n\n",levanto);
	return 0;
}


