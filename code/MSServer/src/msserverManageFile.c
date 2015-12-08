#include "../include/msserverManageFile.h"

fileMovieT clienteDatos;
int inicializado =0;

int 
initFileMovie(char * name,int nivel)
{
	if(strcmp("Invitado",name)==0)
		return 0;
	if(inicializado)
		return 1;
	inicializado=1;
	if(!existFile(name))
	{
		clienteDatos.level=nivel;
		clienteDatos.cantMovie=0;
		clienteDatos.conectado=0;
		clienteDatos.peliculas=NULL;
		return 1;
	}
	else
	{
		return loadFile(name);
	}
}


int
conectarUsuario(void)
{
	if(!inicializado)
		return 0;
	else
		return clienteDatos.conectado=1;
}
int
desconectarUsuario(void)
{
	if(!inicializado)
		return 0;
	else
	{
		clienteDatos.conectado=0;
		return 1;
	}
}

/*Devuelve true si el archivo existe o no*/
int
existFile(char * name)
{
	char nameFile[MAX_NAME + 10];
	FILE * aux;
	if(name==NULL)
		return 0;
	sprintf(nameFile,"%s%s",name,"File.DATA");
	
	if((aux=fopen(nameFile,"r"))==NULL)
		return 0;	
	fclose(aux);
	return 1;
}

/*typedef struct{
	int level;
	int conectado;
	int cantMovie;
	infoMovieFileT * datos;
}fileMovieT;

typedef struct{
	int cantDescargadas;
	char movieName[MAX_NAME];
}infoMovieFileT;*/
int
estoyConectado(void)
{
	if(inicializado)
		return clienteDatos.conectado==1;
	else
		return 0;
}
int
agregarPelicula(char * movieName,int nroTicket)
{
	infoMovieFileT * peliData;
	infoMovieFileT * pelisaux;
	char * nameFilm= toLower(movieName);
  if(existeLaPeli(nameFilm))
		return 0;
	if((peliData=malloc(sizeof(infoMovieFileT)))==NULL)
	{
		fprintf(stderr,"Error general de memoria.\n");
		exit(1);
	}
	clienteDatos.cantMovie++;
	peliData -> cantDescargadas=0;
	strcpy(peliData->movieName,nameFilm);
	peliData -> ticket = nroTicket;
	if((pelisaux=realloc(clienteDatos.peliculas,clienteDatos.cantMovie*sizeof(infoMovieFileT)))==NULL)
	{
		free(clienteDatos.peliculas);
		fprintf(stderr,"Error general de memoria.\n");
		exit(1);
	}
	clienteDatos.peliculas=pelisaux;
	memcpy(&(clienteDatos.peliculas[clienteDatos.cantMovie-1]),peliData,sizeof(infoMovieFileT));
	free(peliData);
	return 1;
}

int
existeLaPeli(char * movieName)
{
	int i=0;
    char * nameFilm=toLower(movieName);
	for(i=0; i< clienteDatos.cantMovie;i++)
	{
		if(strcmp(nameFilm,(clienteDatos.peliculas[i]).movieName)==0)
			return 1;
	}
	return 0;
}
/*int 
puedoDescargarPeli(char * nameFilm)
{
	if(!existeLaPeli(nameFilm))
		return 0;
	int i=0;
	for(i=0; i< clienteDatos.cantMovie;i++)
	{
		if(strcmp(nameFilm,(clienteDatos.peliculas[i]).movieName)==0)
		{
			if(clienteDatos.peliculas[i].cantDescargadas<clienteDatos.level)
				return 1;
			else
				return 0;
		}
	}
	return 0;
}
*/
void
sacarPeli(char * movieName)
{
	infoMovieFileT* aux;
	int i=0;
	int j=0;
    char * nameFilm=toLower(movieName);
	if((aux=malloc((clienteDatos.cantMovie-1)*sizeof(infoMovieFileT)))==NULL)
	{
		fprintf(stderr,"Error fatal de memoria.\n");
		exit(1);
	}
	for(i=0; i< clienteDatos.cantMovie;i++)
	{
		if(strcmp(nameFilm,(clienteDatos.peliculas[i]).movieName)!=0)
		{
			memcpy((&aux[j]),&(clienteDatos.peliculas[i]),sizeof(infoMovieFileT));
			j++;
		}
	}
	free(clienteDatos.peliculas);
	clienteDatos.peliculas=aux;
	return;
}

int
descargarPelicula(char * movieName)
{
	int i=0;
    char * nameFilm=toLower(movieName);
	for(i=0; i< clienteDatos.cantMovie;i++)
	{
		if(strcmp(nameFilm,(clienteDatos.peliculas[i]).movieName)==0)
		{
			if(clienteDatos.peliculas[i].cantDescargadas<clienteDatos.level-1)
			{
				clienteDatos.peliculas[i].cantDescargadas++;
				return 1;
			}
			else if(clienteDatos.peliculas[i].cantDescargadas==clienteDatos.level-1)
			{
				sacarPeli(nameFilm);
				clienteDatos.cantMovie--;
				return 1;
			}
			else{
				return 0;
			}
		}
	}
	return 0;
}


int
loadFile(char * name)
{
	fileMovieT * resp;
	char buffer[MAX_BUFF];
	FILE * source;
	char nameFile[MAX_NAME + 10];
	int linea=0;
	int indice=0;
	if(name==NULL)
		return 0;
	sprintf(nameFile,"%s%s",name,"File.DATA");
	
	if((source=fopen(nameFile,"r"))==NULL)
		return 0;
	/*
	if((resp = malloc(sizeof(fileMovieT)))==NULL)
	{
		fclose(source);
		fprint(stderr,"Error fatal de memoria.\n");
		exit(1);
	}*/
	
	while(fgets(buffer, MAX_BUFF-1, source)!=NULL)
	{
		buffer[strlen(buffer)-1]= '\0';
		switch(linea){
		case 0:
				if(sscanf(buffer,"%d",&clienteDatos.level)!=1)
				{
					fprintf(stderr,"Error fatal de procesamiento.\n");
					fclose(source);
					return 0;
				}
				break;
		case 1:
				if(sscanf(buffer,"%d",&clienteDatos.conectado)!=1)
				{
					fprintf(stderr,"Error fatal de procesamiento.\n");
					fclose(source);
					return 0;
				}
				break;
		case 2:
				if(sscanf(buffer,"%d",&clienteDatos.cantMovie)!=1)
				{
					fprintf(stderr,"Error fatal de procesamiento.\n");
					fclose(source);
					return 0;
				}
				if(clienteDatos.cantMovie==0)
					clienteDatos.peliculas=NULL;
				else if((clienteDatos.peliculas=malloc(clienteDatos.cantMovie*sizeof(infoMovieFileT)))==NULL)
				{
					fprintf(stderr,"Error fatal de memoria.\n");
					fclose(source);
					return 0;
				}
				break;
		default: if(linea%3==0)
								strcpy(clienteDatos.peliculas[indice].movieName,buffer);
						else if(linea%3==1)
						{
							if(sscanf(buffer,"%d",&(clienteDatos.peliculas[indice].cantDescargadas))!=1)
							{
								fprintf(stderr,"Error fatal de procesamiento.\n");
								fclose(source);
								return 0;
							}
							if(clienteDatos.peliculas[indice].cantDescargadas<0)
							{
								fprintf(stderr,"Error fatal de procesamiento.\n");
								fclose(source);
								return 0;
							}
						}
						else
						{
							if(sscanf(buffer,"%d",&(clienteDatos.peliculas[indice].ticket))!=1)
							{
								fprintf(stderr,"Error fatal de procesamiento.\n");
								fclose(source);
								return 0;
							}
							if(clienteDatos.peliculas[indice].ticket<0)
							{
								fprintf(stderr,"Error fatal de procesamiento.\n");
								fclose(source);
								return 0;
							}
							indice++;
						}
						break;
		}
		linea ++;
	}
	if(clienteDatos.cantMovie!=(indice))
	{
		fprintf(stderr,"Error fatal de procesamiento del archivo FILE.DATA.\n");
		fclose(source);
		return 0;
	}
	fclose(source);
	return 1;
}
/*Formato del archivo de guardado
Archivo:
LalaFile.DATA:
*Nivel
*Si esta conectado o no
*Cantidad de peliculas
*Nombre de la primera pelicula
*Cantidad de veces que descargo la primera pelicula
*Ticket
*Nombre de la segunda Pelicula
*Cantidad de veces que descargo la segunda pelicula
*/


int
releaseFileMovie(char *name)
{
	if(strcmp(name,"Invitado")==0)
	{
		return 2;
	}
	if(!inicializado)
	{
		return 0;
	}
	return releaseFile(&clienteDatos,name);
}

int 
releaseFile(fileMovieT * datos,char * name)
{
// 	 size_t fwrite( const void *ptr, size_t tam, size_t nmiemb, FILE *flujo);
	int indice=0;
	FILE * output;
	char nameFile[MAX_NAME + 10];
	sprintf(nameFile,"%s%s",name,"File.DATA");
	if((output=fopen(nameFile,"w"))==NULL)
		return 0;
	char buffer[MAX_BUFF];
	sprintf(buffer,"%d\n",datos->level);
	fwrite(buffer,strlen(buffer)*sizeof(char),1,output);
	sprintf(buffer,"%d\n",datos->conectado);
	fwrite(buffer,strlen(buffer)*sizeof(char),1,output);
	sprintf(buffer,"%d\n",datos->cantMovie);
	fwrite(buffer,strlen(buffer)*sizeof(char),1,output);
	while(indice < datos->cantMovie)
	{
		sprintf(buffer,"%s\n",datos->peliculas[indice].movieName);
		fwrite( buffer, strlen(buffer)*sizeof(char), 1,output);
		sprintf(buffer,"%d\n",datos->peliculas[indice].cantDescargadas);
		fwrite(buffer,strlen(buffer)*sizeof(char),1,output);
		sprintf(buffer,"%d\n",datos->peliculas[indice].ticket);
		fwrite(buffer,strlen(buffer)*sizeof(char),1,output);
		indice ++;
	}
	return 1;
}
