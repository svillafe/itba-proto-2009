#include "../include/msclientListGen.h"

extern char ipServer[MAX_IP];
extern char portNumber[MAX_PORT];
extern char clientName[MAX_NAME];
extern char pass[MAX_PASS];

int
listGens(void)
{

	/*char * pass = "lalalala";*/
	ListGenT* genAux;
  int sock;
  int intentos = 0;

  printf("\n\n--Listar Generos--\n");

	comunicationT msgGen, receiveAns;

	/*Aviso que quiero lsitar las peliculas. Dejo el opData vacio*/
  msgGen.opcode = LIST_GEN;
	strcpy(msgGen.name, clientName);

	if((msgGen.opData=malloc(sizeof(ListGenT)))==NULL)
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    return 0;
  }

	if( (sock = connectTCP(ipServer, portNumber)) < 0 )
	{
			fprintf(stderr,"Error de conexion\n\n");
			free(msgGen.opData);
			return 0;
	}

	((ListGenT*)(msgGen.opData))->cantGen = 0;


	/*Pido la lista de generos*/
	if( enterMsgCom(&msgGen,sock,NO_BLOCK,pass) < 1 )
	{
			printf("\n\nError al mandarle informacion al servidor.\n\n");
			free(msgGen.opData);
			close(sock);
			return 0;
	}

	int cant;

	do
	{
		/*Recibo la respuesta del servidor.*/
		if(getMsgCom(&receiveAns,sock,NO_BLOCK,pass) < 1)
		{
			printf("\n\nError al recibir la informacion del servidor.\n\n");
			free(msgGen.opData);
			close(sock);
			return 0;
		}
		genAux = (ListGenT*)(receiveAns.opData);
		cant = genAux->cantGen;

		/*si cant es 1 tengo que listar el genero*/
		if(cant > 0)
			printf("\nGenero: %s\n",(genAux->gen));

	}while(cant > 0);

  close(sock);
	free(msgGen.opData);
	return 1;
}
