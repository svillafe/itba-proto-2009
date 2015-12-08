#ifndef _PASSIVESOCK_H
#define _PASSIVESOCK_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <netdb.h>
#define PASSIVE_ERROR -1
int passivesock(const char * ipNumner,const char *service, const char *transport, int qlen);
int passiveTCP(const char * ipNumner,const char *service, int qlen);
int passiveUDP(const char * ipNumner,const char *service);
#endif









