#include "../include/msclientHelp.h"

void
prntHelp(void)
{

	printf("\nComandos disponibles\n\n");
	
	printf("conectar <usuario> -- Conecta al servidor con el nombre de usuario ingresado\n");
	printf("pass -- Cambiar clave\n");
	printf("descargar <pelicula> -- Descarga una pelicula\n");
	printf("comprar <pelicula> -- Comprar una pelicula\n");
	printf("lsgen -- Listar generos\n");
	printf("lsfilm -- Listar peliculas\n");
  printf("desconectar -- Desconectar usuario del servidor\n");
  printf("salir -- Cierra el cliente\n\n");
}
