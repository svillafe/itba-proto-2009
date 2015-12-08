#include "../include/connectionclose.h"
void
closeConnection(int fd,char * servidor)
{
  printf("Cerrando la conexion del %s...\n",servidor);
  close(fd);
  printf("Conexion cerrada\n");
}

