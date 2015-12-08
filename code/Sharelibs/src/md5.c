#include "../include/md5.h"

unsigned char *
getMd5(char * name)
{
  FILE * aux;
  char * src;
  char * put;
  char c;
  
  int copiados =0;
  unsigned char * dest=malloc(16*sizeof(char));
  int longitud = 0;
  int i=0;
  
  FILE * archivo;
  char auxName[MAX_LEN_MD5];
  sprintf(auxName,"%s%s",name,"md5File");
  
  
  if((archivo=fopen(auxName,"r"))!=NULL)
  {
    
    if(fread(dest,sizeof(char),16,archivo)!=0)
    {
      fclose(archivo);
      return dest;
    }
    else
        fprintf(stderr,"Error: No se pudo leer del archivo auxiliar del Md5.\n");
    fclose(archivo);
  }

  if((aux = fopen(name,"rb"))==NULL)
  {
    fprintf(stderr,"Fallo el md5, archivo o nombre incorrecto.\n");
    return NULL;
  }
  if((src=malloc(BLOQUE))==NULL)
  {
    fprintf(stderr,"Error fatal, de memoria.\n");
    exit(1);
  }
  longitud=10;

  while((fread(&c,1,1, aux)!=0))
  {
    src[copiados]=c;
    copiados++;
    if(copiados==longitud)
    {
      if((put=realloc(src,longitud=(2*longitud)))==NULL)
      {
        fprintf(stderr,"Error fatal, de memoria.\n");
        exit(1);
      }
      src=put;
    }
  }
  src=realloc(src,copiados);
  MD5(src,copiados,dest);
  
  
  if((archivo=fopen(auxName,"w"))!=NULL)
  {
    if(fwrite(dest,sizeof(char),16,archivo)!=16)
    {
      fprintf(stderr,"Error:Fallo la escritura del archivo auxiliar del md5.\n");
    } 
    
    fflush(archivo);
    fclose(archivo);
  }
  
  fclose(aux);
  return dest;
}

void
eraseMd5File(char * name)
{
   char nombre[MAX_LEN_MD5];
   FILE *fichero;

   sprintf(nombre,"%s%s",name,"md5File");
   fichero = fopen( nombre, "r" ); 
   if( fichero != NULL )
   {
      fclose( fichero );
      remove(nombre); 
      
   }
}

char *
md5ToString(const unsigned char * md5)
{
  int i=0;
  int j=0;
  char * resp;
  if((resp=malloc(LONG_BUFFER_MD5))==NULL)
  {
    fprintf(stderr,"Error fatal de memoria.\n");
    exit(1);
  }
  for(i=0; i<16 ; i++,j++)
  {
    sprintf(&(resp[j]),"%x",md5[i]);
    j++;
  }
  return resp;
}



