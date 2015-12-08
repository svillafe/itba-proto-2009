#ifndef _BUY_MOVIE_H
#define _BUY_MOVIE_H
#include "../include/msserver.h"
#include "../include/msserverLib.h"
#include "../include/msserverTTL.h"
#include "../../Sharelibs/include/connect.h"
#include "../../Sharelibs/include/passivesock.h"
#include "../../Sharelibs/include/connectionclose.h"
#include "../../Sharelibs/include/slib.h"
#include "../../Sharelibs/include/msgPLServer.h"
#include "msserverManageFile.h"

int buyMovie(comunicationT * mensaje, int fd);


int connectToPServer(endPoint*ps,psMsgRequestT * msg,psMsgReplyT * resp);
int connecToPlserver(char * pServer,endPoint * ps);
int moneyInAccount(char * pServer, double precio, char * usuario, int accountNumber);




#endif
