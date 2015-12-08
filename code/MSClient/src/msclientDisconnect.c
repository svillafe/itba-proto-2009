#include "../include/msclientDisconnect.h"



extern char ipServer[MAX_IP];
extern char portNumber[MAX_PORT];
extern char clientName[MAX_NAME];
extern char pass[MAX_PASS];

int
disconnectUser(void)
{
	
	DisconnectT * disc;
  int sock;
  int intentos = 0;

  printf("\n\n--Desconectar--\n");

	comunicationT msgDisc, receiveAns;

	/*Aviso que quiero lsitar las peliculas. Dejo el opData vacio*/
  msgDisc.opcode = DISCONNECT;
	strcpy(msgDisc.name, clientName);
	
	if((msgDisc.opData=malloc(sizeof(DisconnectT)))==NULL)
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    return 0;
  }

	if( (sock = connectTCP(ipServer, portNumber)) < 0 )
	{
			fprintf(stderr,"Error de conexion\n\n");
			free(msgDisc.opData);
			return 0;
	}

	((DisconnectT *)(msgDisc.opData))->cod = 1;


	/*Aviso que me quiero desconectar al servidor*/
	if( enterMsgCom(&msgDisc,sock,NO_BLOCK,pass) < 1 )
	{
			printf("\n\nError al enviar información.\n");
			free(msgDisc.opData);
			close(sock);
			return 0;
	}

	/*Recibo la respuesta y veo que este todo bien*/

	if(getMsgCom(&receiveAns,sock,NO_BLOCK,pass) < 1)
	{
		printf("\n\nError al recibir la informacion del servidor.\n\n");
		free(msgDisc.opData);
		close(sock);
		return 0;
	}

	if( ((DisconnectT *)(receiveAns.opData))->cod != 1 )
	{
		fprintf(stderr,"El servidor no puedo desconectar el usuario,");
		fprintf(stderr," intentelo mas tarde\n");
		free(msgDisc.opData);
		close(sock);
		return 0;
	}

	free(msgDisc.opData);
	close(sock);
	
	return 1;

}
