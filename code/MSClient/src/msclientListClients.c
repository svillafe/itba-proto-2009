#include "../include/msclientListClients.h"


extern char ipServer[MAX_IP];
extern char portNumber[MAX_PORT];
extern char clientName[MAX_NAME];
extern char pass[MAX_PASS];

int
listClients()
{
	

	ListClientT_Send * clienteAux;
  int sock;
  int intentos = 0;

  printf("\n\n--Listar Clientes--\n");
    
    
  comunicationT msgList, receiveAns, preList;
        
  preList.opcode = PRE_LIST_CLIENTS;
	strcpy(preList.name, clientName);
	preList.opData = NULL;


	if( (sock = connectTCP(ipServer, portNumber)) < 0 )
	{
			fprintf(stderr,"Error de conexion\n\n");
			return 0;
	}

	
	/*Pido la lista de usuarios, y además le digo al servidor que los datos
	 * me los tiene q mandar encriptados*/
	if( enterMsgCom(&preList,sock,NO_BLOCK,pass) < 1 )
	{
			printf("\n\nError al mandarle informacion al servidor.\n\n");
			close(sock);
			return 0;
	}

	int cant;
	
	do
	{
		/*Recibo la respuesta del servidor.*/
		
		if(getMsgCom(&receiveAns,sock,BLOCK,pass) < 1)
		{
			printf("\n\nError al recibir la informacion del servidor.\n\n");
			close(sock);
			return 0;
		}
		clienteAux = (ListClientT_Send *)(receiveAns.opData);
		cant = clienteAux->clientCant;

		/*Tengo un cliente y lo tengo que listar en pantalla*/
		if(cant > 0)
		{
			printf("\nNombre: %s",(clienteAux->cliente).name);
			printf("\nMail: %s",(clienteAux->cliente).mail);
			printf("\nLevel: %d",(clienteAux->cliente).level);
			printf("\nDescripcion: %s\n",(clienteAux->cliente).description);
		}
		

	}while(cant > 0);

  close(sock);

	return 1;
}
