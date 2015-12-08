#ifndef _DOWNLOAD_H
#define _DOWNLOAD_H
#include "../include/msserver.h"
#include "../include/msserverLib.h"
#include "../../Sharelibs/include/connect.h"
#include "../../Sharelibs/include/passivesock.h"
#include "../../Sharelibs/include/connectionclose.h"
#include "../../Sharelibs/include/slib.h"
#include "msserverManageFile.h"


int downloadData(comunicationT * mensaje, int fd, char * path);
int downloadInfo(comunicationT * mensaje, int fd);
int getPath(char * path, char * movieName);
int getExt(char * ext, char * path);

#endif
