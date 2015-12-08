#include "../include/msserverDisconnectUser.h"

/*Funciones de los posibles opcodes que me manden*/
/*Lo que viene de mensaje: Usuario*Contraseña* */
int
disconnectUser(comunicationT * mensaje, int fd)
{
 
	char * pass = "lalalala22";
	/*char *pass[MAX_PASS];*/
	DisconnectT * disc;

  printf("\n\n--Desconectando usuario--\n");

	comunicationT msgDisc, receiveAns;
	
	msgDisc.opcode = DISCONNECT;
	strcpy(msgDisc.name, mensaje->name);
	
	if((msgDisc.opData=malloc(sizeof(DisconnectT)))==NULL)
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    exit(1);
  }
	

	if(!initSerCon(mensaje->name))
  {
  	fprintf(stderr,"No se pudo validar la identidad del usuario %s. Se cierra proceso.",mensaje->name);
		((DisconnectT *)(msgDisc.opData))->cod = 0;
		if(enterMsgCom(&msgDisc,fd,NO_BLOCK,pass) < 1)
		{
			printf("\n\nError al enviar la informacion\n");
			free(msgDisc.opData);
			exit(1);
		}
		free(msgDisc.opData);
		exit(1);
  }

	/*if (getUserPswd(mensaje->name,pass))
		{
			fprintf(stderr,"No se pudo obtener la clave del usuario %s",mensaje->name);
			exit(1);
		}
*/
	int nivel;
	if(getUserLevel(mensaje->name,&nivel)!=0)
	{
		fprintf(stderr,"Error al obtener el nivel del LDAP.\n");
		((DisconnectT *)(msgDisc.opData))->cod = 0;
		if(enterMsgCom(&msgDisc,fd,NO_BLOCK,pass) < 0)
		{
			printf("\n\nError al enviar la informacion del servidor.\n\n");
			free(msgDisc.opData);
			realeaseServer(mensaje->name);			
			exit(1);
		}
		free(msgDisc.opData);
		realeaseServer(mensaje->name);
		exit(1);
	}
	
  if(!initFileMovie(mensaje->name,nivel))/*El 5 es el nivel que debe ser obtenido del LDAP*/
  {
		fprintf(stderr,"Error al cargar el archivo de FILE.DATA\n");
		((DisconnectT *)(msgDisc.opData))->cod = 0;
		if(enterMsgCom(&msgDisc,fd,NO_BLOCK,pass) < 0)
		{
			printf("\n\nError al enviar la informacion del servidor.\n\n");
			free(msgDisc.opData);
			realeaseServer(mensaje->name);			
			exit(1);
		}
		free(msgDisc.opData);
		realeaseServer(mensaje->name);
		exit(1);
  }

	if(desconectarUsuario() != 1)
	{
		fprintf(stderr,"No se pudo desconectar al usuario %s\n\n", mensaje->name);
		((DisconnectT *)(msgDisc.opData))->cod = 0;
		if(enterMsgCom(&msgDisc,fd,NO_BLOCK,pass) < 0)
		{
			fprintf(stderr,"\n\nError al enviar la informacion del servidor.\n\n");
			free(msgDisc.opData);
			releaseFileMovie(mensaje->name);
			realeaseServer(mensaje->name);
			exit(1);
			
		}
		free(msgDisc.opData);
		releaseFileMovie(mensaje->name);
		realeaseServer(mensaje->name);
		exit(1);
	}


	((DisconnectT *)(msgDisc.opData))->cod = 1;
	if(enterMsgCom(&msgDisc,fd,NO_BLOCK,pass) < 1)
	{
		((DisconnectT *)(msgDisc.opData))->cod = 0;
		fprintf(stderr,"\n\nError al enviar la informacion\n");
		free(msgDisc.opData);
		releaseFileMovie(mensaje->name);
		realeaseServer(mensaje->name);
		exit(1);
	}

	
	free(msgDisc.opData);
	releaseFileMovie(mensaje->name);
	realeaseServer(mensaje->name);
  exit(1);
}






