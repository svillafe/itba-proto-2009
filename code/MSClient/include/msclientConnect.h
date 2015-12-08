#ifndef _MS_CLIENT_CONNECT_H
#define _MS_CLIENT_CONNECT_H

#include "../../Sharelibs/include/slib.h"
#include "../../Sharelibs/include/msgsock.h"
#include "../../Sharelibs/include/codes.h"
#include "../../Sharelibs/include/opStructs.h"


int connectUsr(void);

int getPass(char *pass);

int getUser(char *user);

int getOp(char *option);

int validCar(char ascii);

#endif
