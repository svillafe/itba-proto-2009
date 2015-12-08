#ifndef _TTL_LIB_H_
#define _TTL_LIB_H_
#include "../../Sharelibs/include/msgPLServer.h"
#define MAX_BUFFER_TTL 50
int saveTTL(plserverMsgT * psData);
int getPSEndpoint(endPoint * resp, char * name);

#endif
