#include "../include/msclientDownload.h"


extern char ipServer[MAX_IP];
extern char portNumber[MAX_PORT];
extern char clientName[MAX_NAME];
extern char pass[MAX_PASS];

int
downloadMovie()
{
	/*char * pass = "lalalala";*/
	char movieName[MAX_NAME+MAX_EXT];
	int ticket = 0;
	int sock;
	int intentos = 0;
	int fd;
	int i = 0;
	int aux;
	
	comunicationT infoMovie, receiveAns, dataMovie;

	printf("\n\n--Usted esta por bajar una pelicula--\n");
	

	do
	{
		printf("\n\nIngrese el nombre de la pelicula que desea bajar .(Recuerde");
		printf(" no incluir el caracter '*' en la pelicula)\n\npelicula: ");
	} while( (!getString(movieName,"la pelicula")) && (intentos < 4) );


	if(intentos == 4)
	{
		printf("\n\n Saliendo de bajar película\n");
		return 0;
	}
	
	printf("\n\nConectandose al servidor para descargar %s.....\n\n", movieName);
	
	
	
  infoMovie.opcode = DOWNINFO;
	strcpy(infoMovie.name, clientName);
	if( (infoMovie.opData = malloc(sizeof(DownInfoT))) == NULL)
	{
		fprintf(stderr,"\n\nNo hay memoria para el malloc\n\n");
		return 0;
	}
	
	/*Matchear el nombre de la pelicula con el ticket*/

	((DownInfoT*)(infoMovie.opData))->ticket = ticket;
	((DownInfoT*)(infoMovie.opData))->codError = 0;
	((DownInfoT*)(infoMovie.opData))->left = 0;
	strcpy(((DownInfoT*)(infoMovie.opData))->name,movieName);

	if( (sock = connectTCP(ipServer, portNumber)) < 0 )
	{
		fprintf(stderr,"Error de conexion\n\n");
		free(infoMovie.opData);
		return 0;
	}

	/*Mando estructura con info para bajar la pelicula*/

	if(enterMsgCom(&infoMovie,sock,NO_BLOCK,pass) < 1)
	{
		fprintf(stderr,"\n\nError al enviar la informacion del servidor.\n\n");
		free(infoMovie.opData);
		close(sock);
		return 0;	
	}

	if(getMsgCom(&receiveAns,sock,BLOCK,pass) < 1)
	{
		fprintf(stderr,"\n\nError al recibir la informacion del servidor.\n\n");
		free(infoMovie.opData);
		close(sock);
		return 0;
	}
		

	free(infoMovie.opData);
	
	if( (((DownInfoT*)(receiveAns.opData))->codError) == -4)
	{
		fprintf(stderr,"No posee esta pelicula, por que ya consumio todas sus posibles descargas o nunca la compro.\n\n", movieName);
		close(sock);
		return 0;
	}
	if( (((DownInfoT*)(receiveAns.opData))->codError) == -5)
	{
		fprintf(stderr,"Error en el servidor. Intentelo mas tarde.\n\n", movieName);
		close(sock);
		return 0;
	}
	if( (((DownInfoT*)(receiveAns.opData))->codError) <0)
	{
		fprintf(stderr,"No se encontro la pelicula en el servidor.\n\n", movieName);
		close(sock);
		return 0;
	}
	pid_t pid;

	pid = fork();

	if(pid < 0)
	{
		fprintf(stderr,"Error fatal en el fork\n\n");
		close(sock);
		return 0;
	}
	if(pid != 0)
	{
		/*soy el padre*/
		close(sock);
		return 1;
	}

	printf("\nPeticion de descarga aceptada. La descarga ha comenzado\n\n");
		
	strcat(movieName,".");
	strcat(movieName,(((DownInfoT*)(receiveAns.opData))->extension));

	FILE * arch;

	if((arch = fopen(movieName,"wb") )== NULL)
	{
		fprintf(stderr,"Error al abrir el archivo %s\n",movieName);
		close(sock);
		exit(1);
	}
	
	do
 	{   /*Este usleep es introducido para no saturar la pc*/
         usleep(250);
		if(getMsgCom(&receiveAns,sock,NO_BLOCK,pass) < 1)
		{
			printf("\n\nError al recibir la informacion del servidor.\n\n");
			close(sock);
			exit(1);
		}

		aux = (((DownDataT *)(receiveAns.opData))->longPart);

		if( aux == LONG_PACKAGE )
		{	
			fwrite((((DownDataT*)(receiveAns.opData))->part),1, (((DownDataT*)(receiveAns.opData))->longPart),arch);
		}
	}while( aux == LONG_PACKAGE );

	fwrite((((DownDataT*)(receiveAns.opData))->part),1, (((DownDataT*)(receiveAns.opData))->longPart),arch);

		/*---------CHEQUEAR CON EL MD5-------------*/
	printf("\nSe termino la descarga con exito!\n");
	fclose(arch);
	printf("Chequeo con el md5\n");

	unsigned char* md5;

	if(getMsgCom(&receiveAns,sock,BLOCK,pass) < 1)
	{
		printf("\n\nError al recibir la informacion del servidor.\n\n");
		close(sock);
		return 0;
	}

	if((md5=getMd5(movieName))==NULL)
	{
		fprintf(stderr,"No se pudo calcula el MD5 en el download del cliente\n");
		fprintf(stderr,"No se puede asegurar que el archivo no esté corrupto\n");
		eraseMd5File(movieName);
		close(sock);
		exit(1);
	}
    eraseMd5File(movieName);
	if( strcmp(md5ToString(md5),md5ToString(((ListMovieT_Send *)(receiveAns.opData))->movie.mdFive) ) != 0 )
	{
		fprintf(stderr,"No coinciden los MD5\n");
		printf("Calculado %s \n",md5ToString(md5));
		printf("Recibido %s \n",md5ToString(((ListMovieT_Send *)(receiveAns.opData))->movie.mdFive));
		fprintf(stderr,"No se puede asegurar que el archivo no esté corrupto\n");
		close(sock);
		exit(1);
	}

	printf("La descarga de %s a finalizado con éxito\n", movieName);


	close(sock);
	exit(1);
}
