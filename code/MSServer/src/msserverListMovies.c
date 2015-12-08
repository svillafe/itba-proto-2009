#include "../include/msserverListMovies.h"

extern listADT peliculas;

int
listMovies(comunicationT * mensaje, int fd)
{
		char * pass= "lalalala";
		/*char pass[MAX_PASS];*/
    comunicationT dataMsg;
    ListMovieT_Send * movieData;
    comunicationT rta;
    
    if(!initSerCon(mensaje->name))
    {
        fprintf(stderr,"No se pudo validar la identidad del usuario %s. Se cierra proceso.",mensaje->name);
				exit(1);
    }

		/*if (getUserPswd(mensaje->name,pass))
		{
			fprintf(stderr,"No se pudo obtener la clave del usuario %s",mensaje->name);
			exit(1);
		}
	*/

    dataMsg.opcode = LIST_MOVIES;
    strcpy(dataMsg.name,mensaje->name);

    if((dataMsg.opData=malloc(sizeof(ListMovieT_Send)))==NULL)
    {
      fprintf(stderr,"Error fatal de memoria.\n");
      realeaseServer(mensaje->name);
			exit(1);
    }

    printf("\nEl servidor comienza a mandar las peliculas\n");

		filmDataT * aux;
		ToBegin(peliculas);

		ListMovieT_Send * item;
		item = (ListMovieT_Send *)(dataMsg.opData);

		char gen[MAX_GENDER];
			
		printf("en el servidor recivo el genero %s\n",(((ListMovieT_Send *)(mensaje->opData))->movie).genero);
		
		strcpy(gen, (((ListMovieT_Send *)(mensaje->opData))->movie).genero);

		while( (aux = (filmDataT*)GetDato(peliculas)) != NULL )
  	{
			if( strcmp((aux->infoFilm).genero, gen ) == 0)
			{
				((ListMovieT_Send *)(dataMsg.opData))->cantFilm = 1;
				((ListMovieT_Send *)(dataMsg.opData))->movie= aux->infoFilm;

				if(enterMsgCom(&dataMsg,fd,NO_BLOCK,pass) < 0)
				{
					printf("\n\nError al enviar la informacion del servidor.\n\n");
					realeaseServer(mensaje->name);
					free(dataMsg.opData);
					exit(1);
				}
			}
		}

		((ListMovieT_Send *)(dataMsg.opData))->cantFilm = 0;

		/*printf("le mando al cliente un 0 en cantfilm diciendo que ya se terminaron las peliculas\n");*/
		if(enterMsgCom(&dataMsg,fd,NO_BLOCK,pass) < 0)
		{
			printf("\n\nError al enviar la informacion del servidor.\n\n");
			realeaseServer(mensaje->name);
			free(dataMsg.opData);
			exit(1);
		}
		

		ToBegin(peliculas);
		
		realeaseServer(mensaje->name);
    free(dataMsg.opData);

    exit(1);
}


