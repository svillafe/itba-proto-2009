#include "../include/passivesock.h"

unsigned short  portbase = 20000;   /* port base, for non-root servers  */
/*------------------------------------------------------------------------
 * passivesock - allocate & bind a server socket using TCP or UDP
 *------------------------------------------------------------------------
 */
int
passivesock(const char * ipNumber, const char *service, const char *transport, int qlen)
/*
 * Arguments:
 *      service   - service associated with the desired port
 *      transport - transport protocol to use ("tcp" or "udp")
 *      qlen      - maximum server request queue length
 */
{
	struct servent	*pse;	/* pointer to service information entry	*/
	struct protoent *ppe;	/* pointer to protocol information entry*/
	struct sockaddr_in sin;	/* an Internet endpoint address		*/
	int	s, type;	/* socket descriptor and socket type	*/

	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;
    if(ipNumber==NULL)
      sin.sin_addr.s_addr = INADDR_ANY;/*Se bindea a todas las interfaces del host*/
    else
      sin.sin_addr.s_addr = inet_addr(ipNumber);
    

    /* Map service name to port number */
	if ( pse = getservbyname(service, transport) )
		sin.sin_port = htons(ntohs((unsigned short)pse->s_port)+ portbase);
	else if ((sin.sin_port=htons((unsigned short)atoi(service))) == 0)
    	return PASSIVE_ERROR;
    
    /* Map protocol name to protocol number */
	if ( (ppe = getprotobyname(transport)) == 0)
	    return PASSIVE_ERROR;
    

    /* Use protocol to choose a socket type */
	if (strcmp(transport, "udp") == 0)
		type = SOCK_DGRAM;
	else
		type = SOCK_STREAM;

    /* Allocate a socket */
	s = socket(PF_INET, type, ppe->p_proto);
	if (s < 0)
		return PASSIVE_ERROR;
    
    /* Bind the socket */
	if (bind(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
	    return PASSIVE_ERROR;
    
	if (type == SOCK_STREAM && listen(s, qlen) < 0)
	   return PASSIVE_ERROR;
    
			
	return s;
}

/*------------------------------------------------------------------------
 * passiveTCP - create a passive socket for use in a TCP server
 *------------------------------------------------------------------------
 */
int
passiveTCP(const char * ipNumber, const char * service, int qlen)
/*
 * Arguments:
 *      service - service associated with the desired port
 *      qlen    - maximum server request queue length
 */
{
	return passivesock(ipNumber, service, "tcp", qlen);
}


/*------------------------------------------------------------------------
 * passiveUDP - create a passive socket for use in a UDP server
 *------------------------------------------------------------------------
 */
int
passiveUDP(const char * ipNumber,const char *service)
/*
 * Arguments:
 *      service - service associated with the desired port
 */
{
	return passivesock(NULL,service, "udp", 0);
}
