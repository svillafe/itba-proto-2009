#ifndef _MS_CLIENT_BUY_H
#define _MS_CLIENT_BUY_H

#include "../../Sharelibs/include/slib.h"
#include "../../Sharelibs/include/msgsock.h"
#include "../../Sharelibs/include/codes.h"
#include "../../Sharelibs/include/opStructs.h"

#define LIMPIA_BUFFER while(getchar()!='\n');
int buyMovie();

int getMovieName(char *movieName);


#endif
