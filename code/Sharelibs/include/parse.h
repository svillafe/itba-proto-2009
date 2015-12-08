#ifndef _PARSE_H
#define _PARSE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "opStructs.h"
#include "msgsock.h"
#include "codes.h"


char * parseaPreBuyMovie(TransportT * msg,int*size);
char * parseaBuyMovie(TransportT* msg,int*size);
char * parseaCon(TransportT* msg,int*size);
char * parseaPreChg(TransportT * data,int * size);
char * parseaChg(TransportT * data,int * size);
char * parseaDownData(TransportT * data,int * size);
char * parseaDownInfo(TransportT * data,int * size);
char * parseaListGen(TransportT * data,int * size);
char * parseaListMovies(TransportT * data,int * size);
char * parseaDisco(TransportT * data,int * size);
char * parseaListClient(TransportT * data,int * size);
char * parseaPreListClient(TransportT * data,int * size);

#endif




































