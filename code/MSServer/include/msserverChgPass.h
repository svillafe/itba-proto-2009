#ifndef _CHG_PASS_H
#define _CHG_PASS_H
#include "../include/msserver.h"
#include "../include/msserverLib.h"
#include "../../Sharelibs/include/connect.h"
#include "../../Sharelibs/include/passivesock.h"
#include "../../Sharelibs/include/connectionclose.h"
#include "../../Sharelibs/include/slib.h"

int chgPass(comunicationT * mensaje, int fd);

#endif
