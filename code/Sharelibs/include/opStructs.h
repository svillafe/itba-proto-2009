#ifndef _OP_STRUCTS_H
#define _OP_STRUCTS_H

/*enum {CON_USR,BUY_FILM,CHG_PASS,DOWNINFO,DOWNDATA,LIST_CLIENT,LIST_GEN,LIST_FILMS,DISCONECT,HELP,EXIT};*/
/*Estructura que se manda para CON_USR*/

/*Constantes*/

#define FIRST_MSG	      	(sizeof(int)+MAX_NAME)
#define CON_MSG	          (MAX_PASS+1)
#define CHG_PASS_MSG      (2*(MAX_PASS) + sizeof(int))
#define BUY_MOVIE_MSG	  	((MAX_NAME+1)+sizeof(int)+sizeof(int)+(MAX_NAME+1))
#define LIST_CLIENTS_MSG  (sizeof(int) + sizeof(clientT *))
#define LIST_GEN_MSG
#define LIST_FILMS_MSG
#define DISCONNECT_MSG			(sizeof(int))
#define DOWNINFO_MSG	  	(3*sizeof(int) + MAX_EXT + MAX_NAME)
#define DOWNDATA_MSG	  	(sizeof(int) + LONG_PACKAGE)
#define CLIENT_MSG        (MAX_NAME + MAX_MAIL + 2*sizeof(int) + sizeof(char*))


#define IP_LEN 16

#define MAX_PORT 10
#define MAX_IP 20
#define MAX_PASS 50
#define MAX_NAME 50
#define MAX_MAIL 50
#define MAX_GENDER 30
#define MAX_MD5 16
#define MAX_EXT 5
#define MAX_PATH 60
#define LONG_PACKAGE 1024
#define MAX_SEND 1024
#define MAX_STRUCT 2048
#define MAX_DESC_GEN 60

typedef struct
{
  char data[MAX_PASS+1];
}conUserT;
/*Estructura que se manda para BUY_FILM*/
typedef struct
{
  char movie[MAX_NAME+1];

/*------------------------------------------*/
/*-campos nuevos agregados para el plServer-*/
  char pServer[MAX_NAME+1];
  int accountNumber;
/*------------------------------------------*/
  
  /*El numero de ticket,
        -1 No tenes plata,
        -2 Tenes mas level para seguir descargando*/
  int ticket;
}buyMovieT;

/*Estructura que se manda para CHG_PAS*/
typedef struct
{
  char oldPass[MAX_PASS];
  char newPass[MAX_PASS];
  int resp;
}chgPassT;

/*Estructura que se manda para DOWNLOAD*/
typedef struct
{
  int ticket;
  int codError;/*El cliente lo manda en cero*/
  int left;/*Niveles que le queda*/
  char extension[MAX_EXT];
  char name[MAX_NAME];
}DownInfoT;

/*Estructura que baja informacion*/
typedef struct
{
	
  int longPart;/*Indica la informaci�n v�lida dentro de part.*/
  char part[LONG_PACKAGE];
}DownDataT;

/*Estructura de un Cliente*/
typedef struct
{
  char name[MAX_NAME];
  char mail[MAX_MAIL];
  int level;
  int longDescription;
  char * description;
}clientT;

/*Estructura de la lista de Clientes*/
typedef struct
{
  int clientCant;
  clientT * lista;
}ListClientT;

typedef struct
{
  int clientCant;
  clientT cliente;
}ListClientT_Send;



/*Estructura de la lista de generos*/
typedef struct
{
  int cantGen;
	char gen[MAX_GENDER];
	/*char descGen[MAX_DESC_GEN];*/
}ListGenT;

/*Estructura de la lista de peliculas*/
typedef struct
{
  char name[MAX_NAME];
  char genero[MAX_GENDER];
  int minutos;
  int megas;
  double precio;
  unsigned char mdFive[MAX_MD5];/*Musica del hombre ara�a*/
  int longDescription;
  char * description;
}MovieT;

/*Estructura de la lista de peliculas*/


// typedef struct
// {
//   int cantFilm;
//   MovieT * lista;   
// }ListMovieT;

typedef struct
{
  int cantFilm;
  MovieT movie;
}ListMovieT_Send;

/*Estructura que desconecta*/
typedef struct
{
  int cod;
}DisconnectT;

#include "msgsock.h"

#endif





































