#include "../include/msclientChgPass.h"

extern char ipServer[MAX_IP];
extern char portNumber[MAX_PORT];
extern char clientName[MAX_NAME];
extern char pass[MAX_PASS];

int
chgPass()
{

    /*char * pass = "lalalala";*/
    char oldPass[MAX_PASS];
    char newPass[MAX_PASS];
    int ticket;
    int sock;
    int intentos = 0;

    printf("\n\n--Cambio de Password--\n");
    
    printf("\nIngrese su password actual.");

    do
    {
      printf("\n\nPassword actual: ");
			intentos++;
    }while( (!getPass(oldPass)) && (intentos < 3) );


    if(intentos == 3)
    {
        printf("\n\n Saliendo de cambiar password\n\n");
        return 0;
    }

		intentos = 0;
    
    printf("\nIngrese su nuevo password.");
        
   do
   {
      printf("\n\nPassword nuevo: ");
      intentos++;
   }while( (getPass(newPass) != 1) && (intentos < 3) );

   if(intentos == 3)
   {
     printf("\n\n Saliendo de cambiar password\n\n");
     return 0;
   }
  
  
    printf("\n\nProcesando el cambio de clave\n\n");
    
    comunicationT msgPass, receiveAns, preChg;
        
    preChg.opcode = PRE_CHG_PASS;
    strcpy(preChg.name,clientName);
    preChg.opData = NULL;

    msgPass.opcode = CHG_PASS;
    strcpy(msgPass.name, clientName);
    if( (msgPass.opData = malloc(sizeof(chgPassT))) == NULL)
    {
        fprintf(stderr,"\n\nNo hay memoria para el malloc\n\n");
        return 0;
    }
    
    strcpy( ((chgPassT*)(msgPass.opData))->oldPass, oldPass );
    strcpy( ((chgPassT*)(msgPass.opData))->newPass, newPass );

    if( (sock = connectTCP(ipServer, portNumber)) < 0 )
    {
        fprintf(stderr,"Error de conexion\n\n");
        free(msgPass.opData);
        return 0;
    }


    
    /*Aviso que el mensaje que sigue es encriptado para que pueda obtener
			la clave del LDAP con el nombre de usuario*/
    if( enterMsgCom(&preChg,sock,NO_BLOCK,pass) < 0 )
    {
        printf("\n\nError al mandarle informacion al servidor.\n\n");
        close(sock);
				free(msgPass.opData);
				return 0;
    }
    
    /*Mando la info con la clave nueva*/
    if( enterMsgCom(&msgPass,sock,NO_BLOCK,pass) < 0 )
    {
        printf("\n\nError al mandarle informacion al servidor.\n\n");
        close(sock);
				free(msgPass.opData);
				return 0;
    }

    /*Recibo la respuesta del servidor.*/    
    if(getMsgCom(&receiveAns,sock,NO_BLOCK,pass) < 0)
    {
      printf("\n\nError al recibir la informacion del servidor.\n\n");
      close(sock);
			free(msgPass.opData);
			return 0;
    }


    switch( ((chgPassT*)(receiveAns.opData))->resp )
    {
        case 1:   printf("\nEl cambio de password ha sido realizado");
                  printf(" con exito.\n");
                  
                  return 1;
        
        case -1:  printf("\nSu clave actual no es correcta.\n");
                  
                  return 0;
        
        case -2:  printf("\nError del servidor.\n");
                  
                  return 0;
    }



    free(msgPass.opData);
    close(sock);

    return 1;



}

