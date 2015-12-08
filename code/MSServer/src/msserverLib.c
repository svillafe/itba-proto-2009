#include "../include/msserverLib.h"


extern int errno;
extern listADT peliculas;
listADT generos;
int ticket =1;

int
compEndPoints(endPoint *ep1,endPoint *ep2)
{
	strcmp(ep1->dirIP,ep2->dirIP);
}


listADT
getData(endPoint* infoPl, endPoint *infoLdap)
{
  FILE * input;
  char buffer[MAX_BUFFER];
  char header[MAX_BUFFER];

  endPoint ep={0};
  int numLinea=0;
  
  int max_port;

  listADT listEP=NULL;
  endPoint *elem;

  if ( (listEP= listNew ( (int(*)(void*,void*))compEndPoints))==NULL)
  {
          fprintf(stderr,"getData: Error al iniciar lista");
          return NULL;
  }

  if((input=fopen("Config/mssconfig","r"))==NULL)
  {
      fprintf(stderr,"No se pudo abrir el archivo de configuracion mssconfig.\n");
      listFree(listEP);
      return NULL;
  }

  while(fgets(buffer,MAX_BUFFER-1,input)!=NULL && *buffer!='\n')
  {
      numLinea++;

      switch(numLinea)
      {
          case 1:
              elem=infoLdap;
              strcpy(header,"ldapServer");
              max_port=0;
              break;
          case 2:
              elem=infoPl;
              strcpy(header,"plServer");
              max_port=1024;
              break;
          default:
              elem=calloc(1,sizeof(endPoint));
              strcpy(header,"mssendpoint");
              max_port=1024;
              break;
      }

      if ((getEndPoint(buffer,elem,header,max_port))==-1)
      {
              fprintf(stderr,"Error en el archivo mssconfig. Linea: %d\n",numLinea);
              fclose(input);
              listFree(listEP);
              return NULL;
      } 

      if (numLinea>2)
          listInsert (listEP, elem);
  }


  fclose(input);

  if (numLinea<2)
  {
    fprintf(stderr,"Error en mssconfig. Insuficientes paramentros\n");
    listFree(listEP);
    return NULL;
  }

  return listEP;
}

int
getEndPoint(char *buffer,endPoint *ep,char *header,int max_port)
{

  char *aux;
  int length;

  length=strlen(buffer);

  if (buffer[length-1]=='\n')
	  buffer[length-1]=';';
  else
  {
	  buffer[length]=';';
	  buffer[length+1]='\0';
  }


  if ((aux=strtok(buffer,"="))==NULL || strcmp(aux,header)!=0)
      return -1;

  if ((aux=strtok(NULL,";"))==NULL || !checkIP(aux))
      return -1;

  strcpy(ep->dirIP,aux);

  int port;

  if ( (aux=strtok(NULL,";"))==NULL || (port=atoi(aux))<=max_port || port>65000 )
	return -1;
  ep->numbPort=port;
  return 0;

}



int
sendString(comunicationT * data,int fd, char * pass)
{
  printf("Opcode:%d\nname:%s\nrespuesta:%s\n",data->opcode,data->name,((conUserT*)(data->opData))->data);
  return enterMsgCom(data, fd, BLOCK, pass);
}

int
comparacion(void * a ,void * b)
{
  filmDataT* elementoA=(filmDataT*)a;
  filmDataT* elementoB=(filmDataT*)b;
  return strcmp(elementoA->infoFilm.name,elementoB->infoFilm.name);
}

int
comparacionGen(void * a ,void * b)
{
  char* elementoA=(char*)a;
  char* elementoB=(char*)b;
  return strcmp(elementoA,elementoB);
}


filmDataT *
parseaBufferFilm(char * buffer)
{
  filmDataT * resp;
  char * ptr;
  int iteracion=0;
  unsigned char * mdfive;
  if((resp=malloc(sizeof(filmDataT)))==NULL)
  {
    fprintf(stderr,"Error Fatal de memoria.\n");
    exit(1);
  }
  ptr=strtok(buffer,"*");
  /*GUARDO LOS NOMBRES EN MINUSCULA*/
  strcpy(resp->infoFilm.name,toLower(ptr));
  iteracion++;
  while((ptr = strtok( NULL, "*" )) != NULL)
  {

    switch(iteracion){
    case 1: strcpy(resp->infoFilm.genero,ptr);
          break;
    case 2: if(sscanf(ptr,"%d",&(resp->infoFilm.minutos))!=1)
            {
              return PARSE_ERROR;
            }
          break;
    case 3: if(sscanf(ptr,"%d",&(resp->infoFilm.megas))!=1)
            {
              return PARSE_ERROR;
            }
          break;
    case 4: if(sscanf(ptr,"%lg",&(resp->infoFilm.precio))!=1)
            {
              return PARSE_ERROR;
            }
          break;
    case 5: if((resp->infoFilm.description=malloc(strlen(ptr)+1))==NULL)
            {
              return PARSE_ERROR;
            }
            strcpy(resp->infoFilm.description,ptr);
            resp->infoFilm.longDescription=strlen(ptr)+1;
            break;
    case 6: strcpy(resp->path,ptr);
          break;
    }
    iteracion++;
  }
  if(iteracion!=7)
  {
    return PARSE_ERROR;
  }
  if((mdfive=getMd5(resp->path))==NULL)
	return PARSE_ERROR;
  memcpy(resp->infoFilm.mdFive,mdfive,16);
  return resp;
}

int
loadGens(void)
{
	filmDataT * aux;
	ToBegin(peliculas);
	char genero[MAX_GENDER];
	char auxG[MAX_GENDER];
	char *auxi;

	if((generos=listNew(comparacionGen))==NULL)
	{
		fprintf(stderr,"Error fatal de libreria lista. Funcion listNew\n");
		exit(1);
	}

	while( (aux = (filmDataT*)GetDato(peliculas)) != NULL )
	{
		listInsert (generos, (aux->infoFilm).genero);
	}

  ToBegin(generos);
  ToBegin(generos);
  ToBegin(peliculas);

	return 1;
}

int
loadFilms(void)
{
  FILE * entrada;
  char buffer[MAX_BUFFER];
  filmDataT * aux;
  int linea=0;
  char * separador="*";
  char * ptr;
  if((peliculas=listNew(comparacion))==NULL)
  {
    fprintf(stderr,"Error fatal de libreria lista. Funcion listNew\n");
    exit(1);
  }
  
  if((entrada=fopen(PATH_FILM,"r"))==NULL)
    return DATA_ERROR;
  while(fgets(buffer,MAX_BUFFER-1,entrada)!=NULL)
  {
    buffer[strlen(buffer)-1]= '\0';
    if((aux=parseaBufferFilm(buffer))==PARSE_ERROR)
    {
      return LOAD_FILMS_ERROR;
    }
    else
    {
			if(listInsert (peliculas, aux)!=1)
      {
        fprintf(stderr,"Error fatal de libreria lista.\n");
        exit(1);
      }
    }
  }
  return 0;
}

int
filmExist(char * movieName)
{
  filmDataT aux;
  strcpy(aux.infoFilm.name,toLower(movieName));
  return listElemBelong(peliculas, &aux);
}	


double
giveMePrice(char * moviEName)
{
  filmDataT* aux;
  double resp=-1;
  char * movieName=toLower(moviEName);
  while((aux=(filmDataT*)GetDato(peliculas))!=NULL)
  {
    if(strcmp(aux->infoFilm.name,movieName)==0)
    {
      resp=aux->infoFilm.precio;
      
    }
  }
  ToBegin(peliculas);
  return resp;
}

int
changePass(comunicationT * mensaje, int fd)
{
  initSerCon(mensaje->name);
    //char * pass=getPassbyName(mensaje->name);
	realeaseServer(mensaje->name);
}

int
getNumberTicket(void)
{	FILE * entrada;
	int ticket=0;
	int resp=0;

	if((entrada=fopen("numberTicket","rb"))==NULL)
  {
		if((entrada=fopen("numberTicket","wb"))==NULL)
			return -1;
		ticket=1;
	}
	else
	{
		fread(&ticket,sizeof(int),1,entrada);
		entrada = fopen("numberTicket","w");
	}	
	resp=ticket;
	ticket = (ticket+1);
	fwrite(&ticket,sizeof(int),1,entrada);
	return resp;
}



/*Las siguiente funciones HACER UN ARCHIVO APARTE AL MOMENTO DE IMPLEMENTARLAS*/
	
 
int
disconect(comunicationT *mensaje,int fd)
{
  initSerCon(mensaje->name);

  realeaseServer(mensaje->name);
}


