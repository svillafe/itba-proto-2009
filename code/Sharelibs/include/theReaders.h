#ifndef _THE_READERS_H
#define _THE_READERS_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "opStructs.h"
#include "encriptar.h"

/*Funcion de Lectura para la opcion de ConUsr*/

void * readConUsr(int fd,int block,int * mcookie,char * psw);

/*Funcion de Lectura para la opcion de PreBuyMovie*/

void * readPreBuyMovie(int fd,int block,int * mcookie, char * psw);

/*Funcion de Lectura para la opcion de BuyMovie*/

void * readBuyMovie(int fd,int block,int * mcookie,char * psw);

/*Funcion de Lectura para la opcion de preChgPass*/

void * readPreChg(int fd,int block,int *mcookie,char * psw);

/*Funcion de Lectura para la opcion de ChgPass*/

void * readChg(int fd,int block,int *mcookie,char * psw);

/*Funcion de Lectura para la opcion de DownLoad*/
/*Creo que no se usa ya que estan las otras 2 funciones*/

void * readDown(int fd,int block,int * mcookie, char * psw);

/*Funcion de Lectura para la opcion de DownData*/

void * readDownData(int fd,int block,int * mcookie, char * psw);

/*Funcion de Lectura para la opcion de DownInfo*/

void * readDownInfo(int fd,int block,int * mcookie, char * psw);

/*Funcion de Lectura para la opcion de ListGen*/

void * readListGen(int fd,int block,int * mcookie, char * psw);

/*Funcion de Lectura para la opcion de preListClient*/

void * readPreListClient(int fd,int block,int * mcookie, char * psw);

/*Funcion de Lectura para la opcion de ListClient*/

void * readListClient(int fd,int block,int * mcookie, char * psw);

/*Funcion de Lectura para la opcion de ListFilm*/

void * readListFilm(int fd,int block,int * mcookie, char * psw);

/*Funcion de Lectura para la opcion de ReadDisco*/

void * readDisco(int fd,int block,int * mcookie, char * psw);

#endif






