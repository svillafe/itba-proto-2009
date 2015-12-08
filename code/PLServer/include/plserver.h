#ifndef _P_L_SERVER_H
#define _P_L_SERVER_H

#include <stdio.h>
#include "../include/plserverLib.h"
#include "../../Sharelibs/include/listGen.h"

//Espera conecciones del Movie Store Server
int cltLookUpSrv(plserverMsgT msg);

//Funcion que se se ejecuta cuando un hijo se cierra
void reaper(int sig);

//Funcion de comparacion para la lista de Payment Servers. Compara por identificador
int compPservers(plserverMsgT * ps1,plserverMsgT * ps2);


#endif

