#ifndef _CONNECT_H
#define _CONNECT_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#ifndef INADDR_NONE
#define INADDR_NONE 0xffffffff
#endif  /* INADDR_NONE */
#define CONNECT_ERROR -1
int connectsock(const char *host, const char *service, const char *transport );
int connectTCP(const char *host, const char *service );
int connectUDP(const char *host, const char *service );
#endif

