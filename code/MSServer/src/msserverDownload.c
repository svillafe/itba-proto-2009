#include "../include/msserverDownload.h"

extern listADT peliculas;

int
downloadInfo(comunicationT * mensaje, int fd)
{
	char * pass= "lalalala";
	/*char pass[MAX_PASS]*/
	DownInfoT * entryData;
	char password[MAX_PASS];
  char movieName[MAX_NAME];
	comunicationT rta;
	char moviePath[MAX_PATH];

	
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


  rta.opcode = DOWNINFO;
  strcpy(rta.name, mensaje->name);
  if((rta.opData=malloc(sizeof(DownInfoT)))==NULL)
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    realeaseServer(mensaje->name);
		exit(1);
  }

	int nivel;
	if(getUserLevel(mensaje->name,&nivel)!=0)
	{
		fprintf(stderr,"Error al obtener el nivel del LDAP.\n");
		((DownInfoT*)(rta.opData))->codError = -5;
		
		if( enterMsgCom(&rta,fd,NO_BLOCK,pass) < 1)
			fprintf(stderr,"Error al enviar la información\n");
		
		realeaseServer(mensaje->name);
		free(rta.opData);
		exit(1);
	}

	if(!initFileMovie(mensaje->name,nivel))
	{
		fprintf(stderr,"Error al cargar el archivo File.DATA");
		((DownInfoT*)(rta.opData))->codError = -5;
		
		if( enterMsgCom(&rta,fd,NO_BLOCK,pass) < 1)
			fprintf(stderr,"Error al enviar la información\n");
		
		realeaseServer(mensaje->name);
		free(rta.opData);
		exit(1);
	}

	/*Hacer el matcheo del numero de ticket con la pelicula correspondiente*/
	strcpy(movieName, (((DownInfoT*)(mensaje->opData))->name));
	/*estoy diciendo que no hubo ningun error y cargo la información para
		la respuesta*/

	
	/*La paso a minuscula y comparo*/
	if(!filmExist(movieName))
	{
		printf("\nNo se encontro la pelicula especificada\n\n");
		((DownInfoT*)(rta.opData))->codError = -1;
		if(enterMsgCom(&rta,fd,NO_BLOCK,pass) < 1)
			fprintf(stderr,"Error al enviar la información\n");
		realeaseServer(mensaje->name);
		releaseFileMovie(mensaje->name);
		free(rta.opData);
		exit(1);
	}

	if( getPath(moviePath, movieName) == 0 )
	{
		fprintf(stderr,"\nError fatal. No se pudo obtener el Path.\n\n");
		((DownInfoT*)(rta.opData))->codError = -2;
		if(enterMsgCom(&rta,fd,NO_BLOCK,pass) < 1)
			fprintf(stderr,"Error al enviar la información\n");
		realeaseServer(mensaje->name);
		releaseFileMovie(mensaje->name);
		free(rta.opData);
		exit(1);
	}	

	if( getExt(((DownInfoT*)(rta.opData))->extension, moviePath) == 0)
	{
		fprintf(stderr,"\nError fatal. No se pudo obtener la extension.\n\n");
		((DownInfoT*)(rta.opData))->codError = -3;
		if(enterMsgCom(&rta,fd,NO_BLOCK,pass) < 1)
			fprintf(stderr,"Error al enviar la información\n");
		realeaseServer(mensaje->name);
		releaseFileMovie(mensaje->name);
		free(rta.opData);
		exit(1);
	}

	if(!existeLaPeli(movieName))/*Se fija que el nivel sea el adecuado*/
	{
		fprintf(stderr,"\nError fatal. No tiene suficiente nivel para descargar esta pelicula.\n\n");
		((DownInfoT*)(rta.opData))->codError = -4;
		if(enterMsgCom(&rta,fd,NO_BLOCK,pass) < 1)
			fprintf(stderr,"Error al enviar la información\n");
		realeaseServer(mensaje->name);
		releaseFileMovie(mensaje->name);
		free(rta.opData);
		exit(1);
	}

	
	((DownInfoT*)(rta.opData))->codError = 1;
	((DownInfoT*)(rta.opData))->left = 4;
	((DownInfoT*)(rta.opData))->ticket = (((DownInfoT*)(mensaje->opData))->ticket);
	strcpy( ((DownInfoT*)(rta.opData))->name, movieName);

	/*le mando la info diciendo todo ok*/
	if(enterMsgCom(&rta,fd,BLOCK,pass) < 1)
	{
		fprintf(stderr,"Error al enviar la información\n");
		realeaseServer(mensaje->name);
		releaseFileMovie(mensaje->name);
		free(rta.opData);
		exit(1);
	}
	
	free(rta.opData);
	downloadData(mensaje, fd, moviePath);
	realeaseServer(mensaje->name);
	
	exit(1);
}

int
downloadData(comunicationT * mensaje, int fd, char * moviePath)
{

  char * pass= "lalalala";
  DownDataT * entryData;
  char password[MAX_PASS];
  char movieName[MAX_NAME];
  comunicationT movieParts;
  int i = 0;
  int control;

	
	
	movieParts.opcode = DOWNDATA;
   strcpy(movieParts.name,mensaje->name);


    if((movieParts.opData=malloc(sizeof(DownDataT)))==NULL)
    { 
      fprintf(stderr,"Error fatal de memoria.\n");
      realeaseServer(mensaje->name);
			exit(1);
    }

	strcpy(movieName, ((DownInfoT*)(mensaje->opData))->name);    	

	

	FILE * arch;
	if( (arch = fopen(moviePath, "rb")) == NULL )
	{
		fprintf(stderr,"\nError al abrir el archivo %s\n",moviePath);
		realeaseServer(mensaje->name);
		free(movieParts.opData);
		exit(1);
	}

	printf("\nComienzo a enviar la pelicula %s\n\n",movieName);
	do
	{  
    usleep(250);
		control = fread((((DownDataT*)(movieParts.opData))->part),1, LONG_PACKAGE,arch);
						
		if(control == LONG_PACKAGE)
		{
			((DownDataT*)(movieParts.opData))->longPart = control;
			if(enterMsgCom(&movieParts,fd,BLOCK,pass) < 0)
			{
				printf("\n\nError al enviar la informacion del servidor.\n\n");
				realeaseServer(mensaje->name);
				free(movieParts.opData);
				fclose(arch);
				exit(1);
			}
		}
		
	}while( control == LONG_PACKAGE );

	/*Envio el ultimo paquete*/

	((DownDataT*)(movieParts.opData))->longPart = control;
	if(enterMsgCom(&movieParts,fd,BLOCK,pass) < 0)
	{
		printf("\n\nError al enviar la informacion del servidor.\n\n");
		realeaseServer(mensaje->name);
		free(movieParts.opData);
		fclose(arch);
		exit(1);
	}
	
	printf("Se termino de enviar la pelicula.\n");

	free(movieParts.opData);

	/*Tengo que mandar info con el md5 de la peli*/
	movieParts.opcode = LIST_MOVIES;

	if((movieParts.opData = malloc(sizeof(ListMovieT_Send))) == NULL )
	{
		fprintf(stderr,"Error de memoria\n");
		realeaseServer(mensaje->name);
		fclose(arch);
		exit(1);
	}

	filmDataT * aux;
	int flag = 0;

	while( ((aux = (filmDataT*)GetDato(peliculas)) != NULL) && !flag )
	{
		if( strcmp( (aux->infoFilm).name, toLower(movieName)) == 0 )
		{
			((ListMovieT_Send *)(movieParts.opData))->cantFilm = 1;
			((ListMovieT_Send *)(movieParts.opData))->movie= aux->infoFilm;
			flag = 1;
		}
	}

	ToBegin(peliculas);

	if(flag == 0)
	{
		fprintf(stderr,"No se encontro la pelicula buscada, %s \n",movieName);
		realeaseServer(mensaje->name);
		free(movieParts.opData);
		fclose(arch);
		exit(1);
	}

	if(enterMsgCom(&movieParts,fd,BLOCK,pass) < 0)
	{
		printf("\n\nError al enviar la informacion del servidor.\n\n");
		realeaseServer(mensaje->name);
		free(movieParts.opData);
		fclose(arch);
		exit(1);
	}

  descargarPelicula(movieName);
  releaseFileMovie(mensaje->name);
	fclose(arch);
	free(movieParts.opData);
	
	exit(1);
}

int
getPath(char * path, char * moviEName)
{
  char * movieName=toLower(moviEName);
  filmDataT * aux;
  ToBegin(peliculas);

  while( (aux = (filmDataT*)GetDato(peliculas)) != NULL )
  {
	 if( strcmp(movieName, (aux->infoFilm).name) == 0 )
	 {
	 	strcpy(path, aux->path);
		ToBegin(peliculas);
        return 1;
        
	 }
  }

  fprintf(stderr,"\nNo se encontro la pelicula en la lista de peliculas\n\n");
  ToBegin(peliculas);

return 0;
}

int
getExt(char * ext,char * path)
{
  
  int i, j, flag = 0;

  for(i=0, j=0; i<strlen(path);i++)
  {
  
    if(flag)
    {
      ext[j] = path[i];
      j++;
    }
    
    if(path[i] == '.')
    flag = 1;
  }

  ext[j]= '\0';

  return 1;
}

