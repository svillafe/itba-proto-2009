#ifndef _SLIB_H
#define _SLIB_H

#define MAX_LEN 256
#define MAX_MSG 512

#define INVALID_CMD -1
#define ERPARAM -2
#define ENTER -3

#include <stdio.h>
#include <string.h>

#include <stdlib.h>

typedef int commandT;

char* toLower(char *movieName);


//Estructura que define un comando para que utilice getCommand
typedef struct
{
	commandT command;
	char name[MAX_LEN+1];
	int cparam; //cantidad de parametros. En este caso siempre es a lo sumo 1
}commandMenuT;



//***ESTRUCTURA A MODIFICAR*****
/*Estructura para mandar mensajes*/
typedef struct
{
      int pid;
      commandT command;
      char msg[MAX_MSG];
}connecT;

/*Funcion que lee un comando de la entrada standard.
**retorna dicho comando, y los argumentos
** La convencion de los argumentos es en el primer char la longitud del primer parametro
** y luego los parametro, como maximo van a haber 2 parametros*/
commandT getCommand(char *prompt,char* param, commandMenuT * cmt, int size);
/*controla que una dirección IP sea válida*/
int controlarString(char*buffer);

/*deja en dir el ip ingresado por el usuario*/
int getIP(char *dir);
/*deja en port el puerto ingresado por el usuario*/
int getPort(char *port);

#endif
