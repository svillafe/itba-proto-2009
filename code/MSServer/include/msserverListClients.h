#ifndef _LIST_CLIENTS_H
#define _LIST_CLIENTS_H

#include "../include/msserver.h"
#include "../include/msserverLib.h"
#include "../../Sharelibs/include/connect.h"
#include "../../Sharelibs/include/passivesock.h"
#include "../../Sharelibs/include/connectionclose.h"
#include "../../Sharelibs/include/slib.h"
#include "../include/msserverLDAP.h"

int listClients(comunicationT * mensaje, int fd);

#endif

