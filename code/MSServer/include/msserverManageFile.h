#ifndef MANAGE_FILE_H
#define MANAGE_FILE_H
#include "../../Sharelibs/include/msgsock.h"
#include "../../Sharelibs/include/slib.h"
#define MAX_BUFF 52


typedef struct{
	int cantDescargadas;
	char movieName[MAX_NAME+1];
	int ticket;
}infoMovieFileT;

typedef struct{
	int cantMovie;
	int conectado;
	int level;
	infoMovieFileT * peliculas;
}fileMovieT;



int  initFileMovie(char * name,int nivel);
int releaseFileMovie(char *name);
int conectarUsuario(void);
int desconectarUsuario(void);
int estoyConectado(void);
/*Devuelve true si el archivo existe o no*/
int existFile(char * name);
int loadFile(char * name);
/*Formato del archivo de guardado
Archivo:
LalaFile.DATA:
*Nivel
*Si esta conectado o no
*Cantidad de peliculas
*Nombre de la primera pelicula
*Cantidad de veces que descargo la primera pelicula
*Nombre de la segunda Pelicula
*Cantidad de veces que descargo la segunda pelicula
*/

int releaseFile(fileMovieT * datos,char * name);

#endif














