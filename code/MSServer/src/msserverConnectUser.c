#include "../include/msserverConnectUser.h"

/*Funciones de los posibles opcodes que me manden*/
/*Lo que viene de mensaje: Usuario*Contraseña* */
int
connectUser(comunicationT * mensaje, int fd)
{


  /*char * pass= "lala";*/
  char pass[MAX_PASS];
	/*printf("Opcode:%d\nname:%s\npass:%s\n",mensaje->opcode,mensaje->name,((conUserT*)(mensaje->opData))->data);*/
  conUserT * entryData = (conUserT *)(mensaje->opData); 
  char password[MAX_PASS]={0};
  char usuario[MAX_NAME]={0};
  char * clave;
  int nivel;
  comunicationT sendData={0};
  
  memcpy(sendData.name,mensaje->name,MAX_NAME);
  sendData.opcode=CON_USR;
  	
  if((sendData.opData=calloc(1,sizeof(conUserT)))==NULL)
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    exit(1);
  }
	
  strcpy(usuario,mensaje->name);

  
  if (getUserPswd(mensaje->name,pass)==-1)
  {
    printf("Entre scs!!!!!!!!!!!!!!!!!\n");
	fprintf(stderr,"No se pudo obtener la clave del usuario %s",mensaje->name);
	strcpy(((conUserT*)(sendData.opData))->data,"KO");
  	sendString(&sendData,fd,NULL);
	exit(1);
  }
  if(strcmp(mensaje->name,"Invitado")!=0)
  {
  	if(getUserLevel(mensaje->name,&nivel)!=0)
		{
			fprintf(stderr,"Error al obtener el nivel del LDAP.\n");
			strcpy(((conUserT*)(sendData.opData))->data,"KO");
			sendString(&sendData,fd,pass);
			free(sendData.opData);
			exit(1);
		}
	
		if(!initFileMovie(mensaje->name,nivel))/*El 5 es el nivel que debe ser obtenido del LDAP*/
		{
			fprintf(stderr,"Error al cargar el archivo de FILE.DATA\n");
			strcpy(((conUserT*)(sendData.opData))->data,"KO");
			sendString(&sendData,fd,pass);
			free(sendData.opData);
			exit(1);
		}
		if(estoyConectado())
		{
			fprintf(stderr,"Usuario ya conectado\n");
			strcpy(((conUserT*)(sendData.opData))->data,"KO");
			sendString(&sendData,fd,pass);
			free(sendData.opData);
			exit(1);
		}
		if((clave = strtok(((conUserT*)(mensaje->opData))->data, "*" ))==NULL)
    {
      strcpy(((conUserT*)(sendData.opData))->data,"KO");
      sendString(&sendData,fd,pass);
    }
    else
    {
      strcpy(password,clave);
      
      if(strcmp(pass, password) == 0)/*Aca debe verificar que los password sean validos*/
      {
        conectarUsuario();
				strcpy(((conUserT*)(sendData.opData))->data,"OK");
        sendString(&sendData,fd,pass);
      }
      else
      {
        strcpy(((conUserT*)(sendData.opData))->data,"KO");
        sendString(&sendData,fd,pass);
      }
    }
  }
  else
  {
    strcpy(((conUserT*)(sendData.opData))->data,"OK");
    sendString(&sendData,fd,pass);
  }

  free(sendData.opData);
	releaseFileMovie(mensaje->name);
	realeaseServer(mensaje->name);
  exit(1);
}
