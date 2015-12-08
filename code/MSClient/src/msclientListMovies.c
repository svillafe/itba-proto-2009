#include "../include/msclientListMovies.h"

extern char ipServer[MAX_IP];
extern char portNumber[MAX_PORT];
extern char clientName[MAX_NAME];
extern char pass[MAX_PASS];

int
listMovies(void)
{

	ListMovieT_Send * movieAux;
  int sock;
  int intentos = 0;
	char gen[MAX_GENDER];	

  printf("\n\n--Listar Peliculas--\n");

	comunicationT msgMovie, receiveAns;


  msgMovie.opcode = LIST_MOVIES;
	strcpy(msgMovie.name, clientName);

    do
    {
    	printf("\n\nDe los géneros listados ingrese el que desea bajar.");
			printf(" Se recomienda primero listar los géneros disponibles.\n");
			printf("genero: ");
    	intentos ++;
    } while((getGender(gen) == 0) && (intentos < 4) );
    
    if(intentos == 4)
    {
        printf("\n\n Saliendo del cliente\n\n");
        return 0;
    }

		
	if((msgMovie.opData=malloc(sizeof(ListMovieT_Send)))==NULL)
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    return 0;
  }

	if( (sock = connectTCP(ipServer, portNumber)) < 0 )
	{
			fprintf(stderr,"Error de conexion\n\n");
			close(sock);
			return 0;
	}

	/*Le indico al servidor que en este mensaje va el género*/
	((ListMovieT_Send *)(msgMovie.opData))->cantFilm = -1;
	strcpy(((ListMovieT_Send *)(msgMovie.opData))->movie.name,"info" );
	strcpy( (((ListMovieT_Send *)(msgMovie.opData))->movie).genero, gen);

	/*Pido la lista de peliculas de el género indicado*/
	if( enterMsgCom(&msgMovie,sock,NO_BLOCK,pass) < 1 )
	{
			printf("\n\nError al mandarle informacion al servidor.\n\n");
			close(sock);
			free(msgMovie.opData);
			return 0;
	}

	int cant;

	do
	{
		/*Recibo la respuesta del servidor.*/
		if(getMsgCom(&receiveAns,sock,NO_BLOCK,pass) < 0)
		{
			printf("\n\nError al recibir la informacion del servidor.\n\n");
			close(sock);
			free(msgMovie.opData);
			return 0;
		}
		movieAux = (ListMovieT_Send *)(receiveAns.opData);
		cant = movieAux->cantFilm;

		/*Si cant es 1 tengo que listar la info de la película*/
		if(cant == 1)
		{
			printf("\nNombre: %s",(movieAux->movie).name);
			printf("\nGenero: %s\n",(movieAux->movie).genero);
			printf("\nMinutos: %d\n",(movieAux->movie).minutos);
			printf("\nTamanio: %d\n",(movieAux->movie).megas);
			printf("\nPrecio: %lg\n",(movieAux->movie).precio);	
			printf("\nDescripcion: %s\n\n",(movieAux->movie).description);
			printf("--------------------------------------------------------------------------------\n");	

		}
		

	}while(cant == 1);

  close(sock);
	free(msgMovie.opData);

	return 1;
}

int getGender(char *resp)
{
	int i;
	
	if(fgets(resp,MAX_GENDER-1,stdin) != NULL)
	{
		for(i=0; i<strlen(resp); i++)
    {	
			if(resp[i] == '*')
			{
				printf("\nNo pongas asteriscos!!\n");
				return 0;
			}
			if(resp[i] == '\n')
				resp[i] = '\0';
		}
		return 1;
	}		
		
	fprintf(stderr,"\n\nError de lectura\n\n");
	return 0;
}
