#include "../include/msserverListGen.h"

extern listADT peliculas;
extern listADT generos;

int
listGens(comunicationT * mensaje, int fd)
{
		char * pass= "lalalala";
		/*char pass[MAX_PASS];*/
    comunicationT dataMsg;
    ListGenT* genData;
    comunicationT rta;

    if(!initSerCon(mensaje->name))
    {
      fprintf(stderr,"No se pudo validar la identidad del usuario %s. Se cierra proceso.",mensaje->name);
			
			exit(1);
    }

		/*if (getUserPswd(mensaje->name,pass))
		{
			fprintf(stderr,"No se pudo obtener la clave del usuario %s",mensaje->name);
			exit(1);
		}
	*/

    dataMsg.opcode = LIST_GEN;
    strcpy(dataMsg.name,mensaje->name);

    if((dataMsg.opData=malloc(sizeof(ListGenT)))==NULL)
    {
      fprintf(stderr,"Error fatal de memoria.\n");
      realeaseServer(mensaje->name);
			exit(1);
    }

    printf("\nEl servidor comienza a mandar los generos\n");

		char *aux;

		loadGens();
		ToBegin(generos);


		while( (aux = (char*)GetDato(generos)) != NULL )
  	{
			((ListGenT*)(dataMsg.opData))->cantGen =1 ;
			strcpy( ((ListGenT*)(dataMsg.opData))->gen, aux);
			/*printf("Leo el genero %s\n",aux);*/
			if(enterMsgCom(&dataMsg,fd,NO_BLOCK,pass) < 0)
			{
				printf("\n\nError al enviar la informacion del servidor.\n\n");
				free(dataMsg.opData);
				realeaseServer(mensaje->name);
				exit(1);
			}

		}

		((ListGenT*)(dataMsg.opData))->cantGen = 0;

		if(enterMsgCom(&dataMsg,fd,NO_BLOCK,pass) < 0)
		{
			printf("\n\nError al enviar la informacion del servidor.\n\n");
			free(dataMsg.opData);
			realeaseServer(mensaje->name);
			exit(1);
		}
		
		ToBegin(peliculas);
		ToBegin(generos);
		realeaseServer(mensaje->name);
    free(dataMsg.opData);

    exit(1);
}
