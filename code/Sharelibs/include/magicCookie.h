#ifndef _MAGIC_COOKIE_H
#define _MAGIC_COOKIE_H

#include "opStructs.h"
#include "msgsock.h"
/*Graba la Magic Cookie una ves terminada una tarea del servidor*/

void realeaseServer(char* nombre);

/*Se encarga de leer el archivo de la MCookie y comparar si la mc que le llego 
 *es la correcta*/

int initSerCon(char* userName);

/*Funciones que inicializan cosas en la capa de "transporte" tanto 
 *del servidor como en el cliente*/
void initCliente(void);
void initServidor(void);

/*------------------------------*/
/*Funciones del servidor y del cliente*/

int serverFun(int oldMaCookie);

int clientFun(int oldMaCookie);
/*------------------------------*/

int setMagicCookie(comunicationT * estructura);

/*Controla la Magic Cookie recibida*/

int checkMCookie(int mc);
#endif

















