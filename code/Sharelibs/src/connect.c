#include "../include/connect.h"

/*------------------------------------------------------------------------
 * connectsock - allocate & connect a socket using TCP or UDP
 *------------------------------------------------------------------------
 */
int
connectsock(const char *host, const char *service, const char *transport )
/*
 * Arguments:
 *      host      - name of host to which connection is desired
 *      service   - service associated with the desired port
 *      transport - name of transport protocol to use ("tcp" or "udp")
 */
{
	struct hostent	*phe = NULL;	/* pointer to host information entry	*/
	struct servent	*pse = NULL;	/* pointer to service information entry	*/
	struct protoent *ppe = NULL;	/* pointer to protocol information entry*/
	struct sockaddr_in sin = {0};	/* an Internet endpoint address		*/
	int	s, type;	/* socket descriptor and socket type	*/


	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;

    /* Map service name to port number */
	if ( (pse = getservbyname(service, transport)) )
		sin.sin_port = pse->s_port;
	else if ((sin.sin_port=htons((unsigned short)atoi(service))) == 0)
		return CONNECT_ERROR;
    
    /* Map host name to IP address, allowing for dotted decimal */
	if ( (phe = gethostbyname(host)) )
		memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
	else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
		return CONNECT_ERROR;
    
    /* Map transport protocol name to protocol number */
	if ( (ppe = getprotobyname(transport)) == 0)
		return CONNECT_ERROR;
    
    /* Use protocol to choose a socket type */
	if (strcmp(transport, "udp") == 0)
		type = SOCK_DGRAM;
	else
		type = SOCK_STREAM;
    
    /* Allocate a socket */
	s = socket(PF_INET, type, ppe->p_proto);
	if (s < 0)
		return CONNECT_ERROR;

    /* ---------------------------------------*/
	/* Coloco el socket como no bloqueante*/
    int flags=0;
	int oldFlags=0;
    if ((flags = fcntl(s, F_GETFL, 0)) == -1)
                return CONNECT_ERROR;
	oldFlags= flags;
    flags = flags | O_NONBLOCK;
    if (fcntl(s, F_SETFL, flags) == -1)
      return CONNECT_ERROR;
     /* ---------------------------------------*/
	


    /* Connect the socket */
	int intentos=0;
	while((connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0) && intentos <3)
	{
	  intentos++;
	  /*Espera un segundo para tratar de conectarse de nuevo*/
	  sleep(1);
	}
	if(intentos==3)
	  return CONNECT_ERROR;
     /*----------------------------------------*/
     /*Vuelvo a colocar el socket como bloqueante*/
	if (fcntl(s, F_SETFL, oldFlags) == -1)
	  return CONNECT_ERROR;
     /* ---------------------------------------*/
    return s;
}


/*------------------------------------------------------------------------
 * connectTCP - connect to a specified TCP service on a specified host
 *------------------------------------------------------------------------
 */
int
connectTCP(const char *host, const char *service )
/*
 * Arguments:
 *      host    - name of host to which connection is desired
 *      service - service associated with the desired port
 */
{
	return connectsock( host, service, "tcp");
}

/*------------------------------------------------------------------------
 * connectUDP - connect to a specified UDP service on a specified host
 *------------------------------------------------------------------------
 */
int
connectUDP(const char *host, const char *service )
/*
 * Arguments:
 *      host    - name of host to which connection is desired
 *      service - service associated with the desired port
 */
{
	return connectsock( host, service, "udp");
}
// #include<stdio.h>
// 
// extern int  errno;



/*------------------------------------------------------------------------
 * connectsock - allocate & connect a socket using TCP or UDP
 *------------------------------------------------------------------------
 */
// int
// connectsock(const char *host, const char *service, const char *transport )
// /*
//  * Arguments:
//  *      host      - name of host to which connection is desired
//  *      service   - service associated with the desired port
//  *      transport - name of transport protocol to use ("tcp" or "udp")
//  */
// {
//     struct hostent  *phe;   /* pointer to host information entry    */
//     struct servent  *pse;   /* pointer to service information entry */
//     struct protoent *ppe;   /* pointer to protocol information entry*/
//     struct sockaddr_in sin; /* an Internet endpoint address     */
//     int s, type;    /* socket descriptor and socket type    */


//     memset(&sin, 0, sizeof(sin));
//     sin.sin_family = AF_INET;
// 
//     /* Map service name to port number */
//     if ( (pse = getservbyname(service, transport)) )
//         sin.sin_port = pse->s_port;
//     else if ((sin.sin_port=htons((unsigned short)atoi(service))) == 0)
//         {
//             printf("can't get \"%s\" service entry\n", service);
//             exit(1);
//           }
// 
//     /* Map host name to IP address, allowing for dotted decimal */
//     if ( (phe = gethostbyname(host)) )
//         memcpy(&sin.sin_addr, phe->h_addr, phe->h_length);
//     else if ( (sin.sin_addr.s_addr = inet_addr(host)) == INADDR_NONE )
//         {
//         printf("can't get \"%s\" host entry\n", host);
//          exit(1);
//           }
// 
//     /* Map transport protocol name to protocol number */
//     if ( (ppe = getprotobyname(transport)) == 0)
//         {
//         printf("can't get \"%s\" protocol entry\n", transport);
//          exit(1);
//           }
// 
//     /* Use protocol to choose a socket type */
//     if (strcmp(transport, "udp") == 0)
//         type = SOCK_DGRAM;
//     else
//         type = SOCK_STREAM;
// 
//     /* Allocate a socket */
//     s = socket(PF_INET, type, ppe->p_proto);
//     if (s < 0)
//         {
//         printf("can't create socket: %s\n", strerror(errno));
//          exit(1);
//           }
// 
//     /* Connect the socket */
//     if (connect(s, (struct sockaddr *)&sin, sizeof(sin)) < 0)
//         {
//         printf("can't connect to %s.%s: %s\n", host, service,
//             strerror(errno));
//          exit(1);
//           }
//     return s;
// }
// 
// 
// /*------------------------------------------------------------------------
//  * connectTCP - connect to a specified TCP service on a specified host
//  *------------------------------------------------------------------------
//  */
// int
// connectTCP(const char *host, const char *service )
// /*
//  * Arguments:
//  *      host    - name of host to which connection is desired
//  *      service - service associated with the desired port
//  */
// {
//     return connectsock( host, service, "tcp");
// }
// 
// /*------------------------------------------------------------------------
//  * connectUDP - connect to a specified UDP service on a specified host
//  *------------------------------------------------------------------------
//  */
// int
// connectUDP(const char *host, const char *service )
// /*
//  * Arguments:
//  *      host    - name of host to which connection is desired
//  *      service - service associated with the desired port
//  */
// {
//     return connectsock( host, service, "udp");
// }
