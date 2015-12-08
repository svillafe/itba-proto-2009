#ifndef _LIST_MOVIES_H
#define _LIST_MOVIES_H

#include "../include/msserver.h"
#include "../include/msserverLib.h"
#include "../../Sharelibs/include/connect.h"
#include "../../Sharelibs/include/passivesock.h"
#include "../../Sharelibs/include/connectionclose.h"
#include "../../Sharelibs/include/slib.h"


int listMovies(comunicationT * mensaje, int fd);

#endif

