#include "../include/msclient.h"

/*extern int sock;*/
extern char clientName[MAX_NAME];
void
desconectar(int sig)
{
  if(strcmp(clientName,"")!=0)
  {
    disconnectUser();
  }
  printf("Desconectado con exito\n");
  exit(1);
}

int
main(void)
{
	
	srand((int) time(NULL)); /*para las magic cookies*/
    (void) signal(SIGINT, desconectar);
	initCliente();
	prntWelcome();
	menu();
	return 0;
}

void
prntWelcome(void)
{
	system("clear");
	printf("\nBienvenido al cliente\n\n");
}

void
menu(void)
{
	
	char param[MAX_LEN+1]={0}; //parametro del comando
	commandT command;
	pid_t pid;

/*{CON_USR,PRE_BUY_MOVIE,BUY_MOVIE,PRE_CHG_PASS,CHG_PASS,DOWNINFO,DOWNLOAD,DOWNDATA,PRE_LIST_CLIENT,LIST_CLIENT,LIST_GEN,LIST_FILMS,DISCONECT,HELP,EXIT};*/

	//aca hay que cambiar en cada uno el comando que corresponda. En cada uno se pone: la constante que corresponda, el nombre del comando, y la cantidad de argumentos que deba recibir. 
	commandMenuT cmt[]={ {CON_USR,"conectar",0},{CHG_PASS,"pass",0},
				{DOWNLOAD,"descargar",0},{BUY_MOVIE,"comprar",0}, {LIST_CLIENTS,"clientes",0},
				{LIST_GEN,"generos",0},{LIST_MOVIES,"peliculas",0},{DISCONNECT,"desconectar",0},{HELP,"ayuda",0},{EXIT,"salir",0} };

	/*controlar que si es invitado no pueda realizar las operaciones que no debe*/
	
	do
	{
		switch (command = getCommand("Cliente:>",param,cmt,sizeof(cmt)/sizeof(commandMenuT)))
		{
			case CON_USR:			if(!strcmp(clientName,""))
												{
                        	if( !connectUsr() )
													{
														*clientName = '\0';
														fprintf(stderr,"Error al conectar un usuario\n");
													}
                        }  
												else
													printf("\nEstá logueado el usuario %s\n", clientName);
												
												break;
			
			case CHG_PASS:   if( (strcmp(clientName,"Invitado") != 0) && (strcmp(clientName,"") != 0) ) 
												{		
													if(!chgPass()) 
														fprintf(stderr,"Error al cambiar el password\n");
												}
												else
													printf("\nNo tiene permisos para cambiar el password\n\n");
												
												break;

			
			case DOWNLOAD:  	if( (strcmp(clientName,"Invitado") != 0) && (strcmp(clientName,"") != 0) ) 
												{	
													if(!downloadMovie()) 
														fprintf(stderr,"No se pudo descargar la película\n");
												}else
													printf("\nNo tiene permisos para descargar una pelicula\n\n");
												
												break;
			
			case BUY_MOVIE:		if( (strcmp(clientName,"Invitado") != 0) && (strcmp(clientName,"") != 0) )
												{	
													if(!buyMovie())
 														fprintf(stderr,"Error al comprar una pelicula\n");
												}
												else
													printf("\nNo tiene permisos para comprar una pelicula\n\n");
												
												break;

			case LIST_CLIENTS:	if( (strcmp(clientName,"Invitado") != 0) && (strcmp(clientName,"") != 0) )
													{
														if(!listClients())
															fprintf(stderr,"No se pudo listar los clientes\n");
													}
													else
														printf("\nNo tiene permisos para ver la informacion de los clientes\n\n");
												
													break;

			
			case LIST_GEN:		if( strcmp(clientName,"") != 0 )
												{	
													if(!listGens())
														fprintf(stderr,"No se pudo listar los géneros\n");
												}
												else
													printf("No hay ningun usuario conectado\n");

												break;
			
			case LIST_MOVIES:	if( strcmp(clientName,"") != 0 )
												{	
													if(!listMovies())
														fprintf(stderr,"No se pudo listar las peliculas\n");
												}
												else
													printf("No hay ningun usuario conectado\n");
												
												break;
			
			case DISCONNECT:	if( strcmp(clientName,"") != 0 )
												{
													if(disconnectUser())
													{
														printf("\nEl usuario %s, ha sido desconectado.\n\n",clientName);
														*clientName = '\0';
													}
													else
														fprintf(stderr,"Error al desconectar el usuario\n");
												}
												else
													printf("No hay ningun usuario conectado\n");
												
												break;
			
			case EXIT:				if( strcmp(clientName,"") == 0 )
												{
													printf("\nSaliendo de la aplicacion.\n\n");
													exit(1);
												}
												
												printf("No se puede salir de la aplicacion, el ususario %s todavia esta conectado.\n",clientName);
												printf("Escriba primero el comando 'desconectar'\n");
													
												break;
	
			case HELP:				prntHelp();
												
												break;
			
			default: 
				if (command!=ENTER)
					printf( command==ERPARAM?"\nArgumento invalido - escriba 'ayuda' para ver comandos disponibles\n":"\nComando invalido - escriba ayuda para ver comandos disponibles\n");
		}
	}
	while ( !((command==EXIT) && (strcmp(clientName,"") == 0)) );
}

