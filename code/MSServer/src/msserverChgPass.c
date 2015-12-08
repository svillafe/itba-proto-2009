#include "../include/msserverChgPass.h"

int 
chgPass(comunicationT * mensaje, int fd)
{
    /*char * pass= "lalalala";*/
		char pass[MAX_PASS];
    comunicationT dataMsg;
    chgPassT * entryData;
    comunicationT rta;
    
    if(!initSerCon(mensaje->name))
    {
        fprintf(stderr,"No se pudo validar la identidad del usuario %s. Se cierra proceso.",mensaje->name);
				exit(1);
    }
    
		if((rta.opData=malloc(sizeof(chgPassT)))==NULL)
    {
      fprintf(stderr,"Error fatal de memoria.\n");
      realeaseServer(mensaje->name);
			exit(1);
    }

		if (getUserPswd(mensaje->name,pass) == -1)
		{
			printf("Usuario Invalido, error fatal\n");
			
			rta.opcode = CHG_PASS;
    	strcpy(rta.name,mensaje->name);
			((chgPassT*)(rta.opData))->resp = -2;

			if( enterMsgCom(&rta,fd,BLOCK,pass) < 0 )
    	{
      	fprintf(stderr,"\n\nError al enviarle informacion al cliente.\n\n");
      	exit(1);  
    	}
			free(rta.opData);
			exit(1);
		}	
		printf("la clave obtenida del LDAP fue %s",pass);
    /*Aca obtengo la clave para despues desencriptar dentro de getMSG*/
    /*En este mensaje me viene la informacion del usuario*/
    if( getMsgCom(&dataMsg,fd,NO_BLOCK,pass) < 1 )
    {
      fprintf(stderr,"\n\nError al recibir la informacion del cliente.\n\n");
      realeaseServer(mensaje->name);
			exit(1);
    }

    entryData = (chgPassT*)(dataMsg.opData);
    rta.opcode = CHG_PASS;
    strcpy(rta.name,dataMsg.name);
  
    

    
    /*Controlar con el LDAP cual es la clave actual para ese usuario y
      fijarse que coincida con la actual. Si es así cambiarla por la
      clave insertada en newPass*/
   	
    if(strcmp(entryData->oldPass, pass ) == 0)
    {
      if (changeUserPswd(mensaje->name,entryData->newPass) != -1)
			{	
				printf("Password cambiada con exito!\n");
        ((chgPassT*)(rta.opData))->resp = 1;
      }
			else
        ((chgPassT*)(rta.opData))->resp = -2;   
    }    
    else
      ((chgPassT*)(rta.opData))->resp = -1;
    
  
    if( enterMsgCom(&rta,fd,BLOCK,pass) < 0 )
    {
      fprintf(stderr,"\n\nError al enviarle informacion al cliente.\n\n");
      exit(1);  
    }

    realeaseServer(mensaje->name);
    free(rta.opData);

    exit(1);
}


