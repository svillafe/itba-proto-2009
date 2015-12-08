#ifndef _CONNECTION_CLOSE_H
#define _CONNECTION_CLOSE_H
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <stdio.h>
void closeConnection(int fd,char * servidor);
#endif



