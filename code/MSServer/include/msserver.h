#ifndef _MS_SERVER_H
#define _MS_SERVER_H

#include "../../Sharelibs/include/listGen.h"
#include "../../Sharelibs/include/msgPaymentServer.h"

#include "../include/msserverLDAP.h"

#include <stdio.h>
#include "../include/msserver.h"



#include "msserverLib.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/signal.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <netinet/in.h>
#include "../../Sharelibs/include/msgsock.h"
#include "../../Sharelibs/include/codes.h"

#include "msserverDownload.h"
#include "msserverBuyMovie.h"
#include "msserverConnectUser.h"
#include "msserverDisconnectUser.h"
#include "msserverChgPass.h"
#include "msserverListClients.h"
#include "msserverListGen.h"
#include "msserverListMovies.h"

#ifndef MAX
#define MAX(x, y)   ((x) > (y) ? (x) : (y))
#endif  /* MAX */

#define KEY_QUEUE 13

#define MAX_BUFFER 512
#define PORT_LEN 13
#define LINE_MSS_FILE 9


#endif

