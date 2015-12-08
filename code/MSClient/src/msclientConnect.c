#include "../include/msclientConnect.h"


int sock=0;
char clientName[MAX_NAME]={0};
char ipServer[MAX_IP];
char portNumber[MAX_PORT];
char pass[MAX_PASS] = {0};

int
connectUsr(void)
{
	int intentos = 0;
	int i;
/*	char pass[MAX_PASS] = {0};*/
	char option[MAX_LEN] = {0};
	
	char opcion;
	

		do{
			printf("\n\n¿Quiere ingresar como un usuario invitado(S/N)?\n");
			intentos++;
		}while( (intentos < 4) && (getOp(option) != 1) );
		
		if(intentos == 4)
		{
			printf("\n\nSaliendo de conectar usuario\n\n");
			return 0;
		}

		intentos = 0;
		
		if( strcmp(option,"n") == 0 )
		{
		
		do{
			printf("\n\nusuario: ");
			intentos++;
		}while( (getUser(clientName) != 1) && (intentos < 4) );
		
		if(intentos == 4)
		{
			printf("\n\nSaliendo de conectar usuario\n\n");
			return 0;
		}

		intentos = 0;

				
		printf("\nIngrese su password.");
		
		do{
			printf("\n\npassword: ");
			intentos++;
		}while( (getPass(pass) != 1) && (intentos < 4) );

		if(intentos == 4)
		{
			printf("\n\n Saliendo de conectar usuario\n\n");
			return 0;
		}

	}
	else
		strcpy(clientName, "Invitado");
		
	intentos = 0;

	do{
		printf("Ingrese el IP del servidor al que se desea conectar: ");
		intentos ++;
	}while( (getIP(ipServer) != 1) && (intentos < 4) );
	
	if(intentos == 4)
	{
		printf("\n\n Saliendo de conectar usuario\n\n");
		return 0;
	}

	intentos = 0;	

	do{
		printf("Ingrese el numero de puerto al que se desea conectar: ");
		intentos ++;
	}while( (getPort(portNumber) != 1) && (intentos < 4) );

	if(intentos == 4)
	{
		printf("\n\n Saliendo de conectar usuario\n\n");
		return 0;
	}


	/*printf("ip %s  port %s", ipServer, portNumber);*/
	if( (sock = connectTCP(ipServer, portNumber)) < 0 )
	{
		fprintf(stderr,"Error de conexion\n\n");
		return 0;
	}

	comunicationT men;
	comunicationT ans;

	men.opcode = CON_USR;
	strcpy(men.name,clientName);
	
	if( (men.opData = malloc(sizeof(conUserT))) == NULL )
	{
		fprintf(stderr,"\n\nNo hay memoria para el malloc\n\n");
		close(sock);
		return 0;
	}
	
	

	strcpy(((conUserT*)men.opData)->data, pass);
	strcat(((conUserT*)men.opData)->data,"*");
	
	
	if( enterMsgCom(&men,sock,NO_BLOCK, pass) < 1)
	{
		fprintf(stderr,"Error al enviar información\n");
		close(sock);
		free(men.opData);
		return 0;
	}


	if(getMsgCom(&ans,sock,NO_BLOCK,pass) < 0)
  {
		printf("Fallo el socket\n");
		close(sock);
		free(men.opData);
		return 0;
	}
		/*printf("\n\nEl servidor me mando %s\n\n",((conUserT*)(ans.opData))->data);*/
		if(strcmp(((conUserT*)(ans.opData))->data,"KO")==0)
		{
			printf("No se pudo conectar con el servidor\n");
			pass[0] = '\0';
			close(sock);
			free(men.opData);
			return 0;
		}
    close(sock);
		free(men.opData);
	
		return 1;
}


int getPass(char *pass)
{
	int i;
	
	if(fgets(pass,MAX_LEN-1,stdin) != NULL)
	{
		for(i=0; i<strlen(pass); i++)
    {
		  if(pass[i] == '*')
			{
				fprintf(stderr,"\nNo pongas asteriscos en el password!!\n");
				return 0;
			}
			if(pass[i] == '\n')
				pass[i] = '\0';
		
		}
		return 1;
	}

	/*tendria que salir de la aplicacion por un error interno*/
	fprintf(stderr,"\n\nERROR EN LA LECTURA DEL PASSWORD\n\n");
	return 0;
}

int getOp(char *option)
{
	int i = 0;
	
	/*printf("entro a getOP\n");
*/
	if(fgets(option,MAX_LEN-1,stdin) != NULL)
	{
		for(i=0; i<strlen(option); i++)
    {	
			if(option[i] == '\n')
				option[i] = '\0';
		}
	}
		option[0] = tolower(option[0]);
		if( (strcmp(option,"s") == 0) || (strcmp(option,"n") == 0) )
			return 1;


	/*tendria que salir de la aplicacion por un error interno*/
	fprintf(stderr,"\n\nError en la lectura de la opcion\n\n");
	return 0;
}

int getUser(char *user)
{
	int i;

	if(fgets(user,MAX_LEN-1,stdin) != NULL)
	{
		for(i=0; i<strlen(user); i++)
		{
			if(!validCar(user[i]))
				return 0;
			if(user[i] == ' ')
			{
				fprintf(stderr,"El nombre no puede tener espacios\n");
				return 0;
			}
			if(user[i] == '\n')
				user[i] = '\0';
		}


		return 1;
	}
	/*tendria que salir de la aplicacion por un error interno*/
	fprintf(stderr,"\n\nError en la lectura del user\n\n");
	return 0;
}

int validCar(char ascii)
{

	if((ascii == '*') || (ascii == 'ñ') || (ascii == 'Ñ')){
		fprintf(stderr,"Caracter %c de tipo invalido\n",ascii);
		return 0;
	}

	if( (ascii == 'á') || (ascii == 'é') || (ascii == 'í') || (ascii == 'ó') || (ascii == 'ú') )
	{
		fprintf(stderr,"Acentuado minuscula %c invalido\n",ascii);
		return 0;
	}

	if( (ascii == 'Á') || (ascii == 'É') || (ascii == 'Í') || (ascii == 'Ó') || (ascii == 'Ú') )
	{
		fprintf(stderr,"Acentuado mayuscula %c invalido\n",ascii);
		return 0;
	}

	return 1;
}

