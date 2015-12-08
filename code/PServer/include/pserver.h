#ifndef _P_SERVER_H
#define _P_SERVER_H

#include <stdio.h>
#include <syslog.h>
#include "../include/pserverLib.h"
#include "../../Sharelibs/include/listGen.h"
#include "../../Sharelibs/include/msgPaymentServer.h"

//Espera conecciones del Movie Store Server
int cltPServer(int fd);

//Funcion que se se ejecuta cuando un hijo se cierra
void reaper(int sig);

//Funcion de comparacion para la lista de Payment Servers.Compara por nombre del cliente
int compPsClients(psClientInfoT * c1,psClientInfoT * c2);

#endif













