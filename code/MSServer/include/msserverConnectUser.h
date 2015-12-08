#ifndef _CONNECT_USER_H
#define _CONNECT_USER_H
#include "../include/msserver.h"
#include "../include/msserverLib.h"
#include "../../Sharelibs/include/connect.h"
#include "../../Sharelibs/include/passivesock.h"
#include "../../Sharelibs/include/connectionclose.h"
#include "../../Sharelibs/include/slib.h"
#include "msserverManageFile.h"
/*int buyMovie(comunicationT * mensaje, int fd);*/
int connectUser(comunicationT * mensaje, int fd);
#endif
