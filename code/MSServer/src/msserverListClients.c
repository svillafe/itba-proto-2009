#include "../include/msserverListClients.h"



int
listClients(comunicationT * mensaje, int fd)
{
	char pass[MAX_PASS];
	comunicationT dataMsg;
	ListClientT listaDeClientes;
	int i;
	/*char *pass = "123";*/

	if(!initSerCon(mensaje->name))
	{
		fprintf(stderr,"No se pudo validar la identidad del usuario %s. Se cierra proceso.",mensaje->name);
		exit(1);
	}
	
	if (!getUserPswd(mensaje->name,pass))
		printf("%s - password: %s\n",mensaje->name,pass);

	dataMsg.opcode = LIST_CLIENTS;
	strcpy(dataMsg.name,mensaje->name);

	if((dataMsg.opData=malloc(sizeof(ListClientT_Send)))==NULL)
	{
		fprintf(stderr,"Error fatal de memoria.\n");
		realeaseServer(mensaje->name);
		exit(1);
	}


	printf("\nEl servidor comienza a mandar la información de los clientes\n\n");

	listADT usersList = getUsers();
	clientT *client;
	ToBegin(usersList);

	while( client=GetDato(usersList) )
	{
		((ListClientT_Send*)(dataMsg.opData))->clientCant = 1;
		((ListClientT_Send*)(dataMsg.opData))->cliente = *client;  
		
		/*printf("\n\n\nmando Nombre: %s",(((ListClientT_Send*)(dataMsg.opData))->cliente).name);
		printf("\n\n\nmando Mail: %s",(((ListClientT_Send*)(dataMsg.opData))->cliente).mail);		
		printf("\n\n\nmando Level: %d",(((ListClientT_Send*)(dataMsg.opData))->cliente).level);
		printf("\n\n\nmando LongDesc: %d",(((ListClientT_Send*)(dataMsg.opData))->cliente).longDescription);	
		printf("\n\n\nmando Desc: %s",(((ListClientT_Send*)(dataMsg.opData))->cliente).description);
*/
		if(enterMsgCom(&dataMsg,fd,NO_BLOCK,pass) < 0)
		{
			printf("\n\nError al enviar la informacion del servidor.\n\n");
			free(client->description);
			free(client);
			listFree(usersList);
			free(dataMsg.opData);
			realeaseServer(mensaje->name);
			exit(1);
		}			
  
		free(client->description);
		free(client);

  }

	listFree(usersList);
	
	

	/*Le envío este último paquete, indicando que ya no hay mas usuarios*/	

	((ListClientT_Send*)(dataMsg.opData))->clientCant = 0;
	
	if(enterMsgCom(&dataMsg,fd,NO_BLOCK,pass) < 0)
	{
		printf("\n\nError al enviar la informacion del servidor.\n\n");
		free(dataMsg.opData);
		realeaseServer(mensaje->name);
		exit(1);
	}

	realeaseServer(mensaje->name);
	free(dataMsg.opData);

	exit(1);

}


