#ifndef _SERVER_LIB_H
#define _SERVER_LIB_H
#include "../../Sharelibs/include/msgsock.h"
#include "msserver.h"

#include "../include/msserver.h"
#include "../include/msserverLib.h"
#include "../../Sharelibs/include/connect.h"
#include "../../Sharelibs/include/passivesock.h"
#include "../../Sharelibs/include/connectionclose.h"
#include "../../Sharelibs/include/slib.h"
#include "../../Sharelibs/include/md5.h"
#include <string.h>
#include <stdio.h>

#define DATA_ERROR -1
#define CS_ERROR -1
#define ISERVER_ERROR -1
#define QLEN 32
#define MAXBUFFER 32
#define MAX_USER 19
#define MAX_PWD 19
#define CANT_FILMS 2
#define LOAD_FILMS_ERROR -1

#define MAX_TOKEN 32

#define PATH_FILM "movies/peliculas.txt"
#define PARSE_ERROR NULL

typedef struct{
  MovieT infoFilm;
  char path[MAX_NAME];
}filmDataT;

/*Funcion que parsea el archivo de configuracion
 *Retorna la informacion del pls y del ldap, y una lista con los puertos e ips locales
*/
listADT getData(endPoint* infoPl, endPoint *infoLdap);
/*Funcion que inicializa la conexion con los servidores*/
int initServers( endPoint * plData, endPoint * ldapData, int * plsocket, int * ldapsocket);
/*Funcion que crea el hijo para atender conexiones nuevas*/
int createConectionSon(int * connectionsock,endPoint * mydata);
/*Funcion que verifica un socket*/
void checkSock(int socket);



#define NOT_FOUND_MOVIE_TICKET -1

/*Funcion que te entrega un numero de ticket*/
int getNumberTicket(void);

/*Funcion que te devuelve 1 si la pelicua exite, 0 en caso contrario*/
int filmExist(char * movieName);

double giveMePrice(char * movieName);

/*Funciones de actividades*/

int changePass(comunicationT * recibo, int fd);
int listGen(comunicationT * mensaje, int fd);
int listFilms(comunicationT * mensaje, int fd);
int disconect(comunicationT *mensaje,int fd);

int sendString(comunicationT* data,int fd,char * pass);
int comparacionGen(void * a ,void * b);
/*Estructura que posee el path y la informacion sobre una pelicula*/
int loadGens(void);
#endif
