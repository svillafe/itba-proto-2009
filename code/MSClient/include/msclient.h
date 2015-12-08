#ifndef _SERVER_IO_H
#define _SERVER_IO_H

#include <stdio.h>
#include <string.h>
#include "../../Sharelibs/include/slib.h"
#include "../../Sharelibs/include/msgsock.h"
#include "../include/msclientConnect.h"
#include "../include/msclientBuy.h"
#include "../include/msclientHelp.h"
#include "../include/msclientDownload.h"
#include "../include/msclientListClients.h"
#include "../include/msclientListMovies.h"
#include "../include/msclientListGen.h"
//comandos validos para el cliente

/*Informacion que tiene que tener el servidor para las peliculas*/
/*
typedef struct
{
	char nameFilm[MAX_NAME];
	char genFilm[MAX_NAME];
	int durationFilm;
	int size;
	int md5;
	double price;
	char * desciption;
}movieT;*/


/*Funcion que imprime la bienvenida al cliente*/
void prntWelcome(void);

/*Funcion que maneja los comandos introducidos por el usuario*/
void menu(void);

/*Funcion que imprime los comandos disponibles*/


/*
//funciones que se encargan de procesar la respuesta del servidor correspondiente a cada peticion del usuario
void s_reg(connecT *con);
void s_usrs(connecT *con);
void s_dir(connecT *con);
void s_last10(connecT *con);
void s_last10usr(connecT *con);
void s_list(connecT *con);
void s_exit(connecT *con);
*/

#endif
