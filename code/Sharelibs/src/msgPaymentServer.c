#include "../include/msgPaymentServer.h"

static void //funcion interna
writeBuffer(char *buffer,char *header,char *info)
{
  strcat(buffer,header);
  strcat(buffer,info);
  buffer[strlen(buffer)]='\n'; //reemplazo el '\0' por '\n'
}


static int //funcion interna
readBuffer(char *buffer,char *header,char *info,int max_info)
{

  char aux_header[MAX_BUFFER_REQUEST]={0};
  int corrector=0;

  strncpy(aux_header,buffer,strlen(header)-1);

  if (strncmp(aux_header,header,strlen(header)-1)!=0)
  {
    fprintf(stderr, "Warning: header deberia ser: %s, pero es %s.\n",header,aux_header);
  }

  if (buffer[strlen(header)-1]==' ') //acepto que haya un espacio entre el header y la informacion
      corrector=1;

  if (max_info==0)
    return READ_OK;

  if (strlen(buffer+strlen(header)-1+corrector)>max_info)
  {
    fprintf(stderr, "Error: informacion demasiado grande\n");
    return READ_ERROR;
  }

  strcpy(info,buffer+strlen(header)-1+corrector);
  
  return READ_OK;
}



int
connectToPS(endPoint *ps, int *psSocket)
{
    char puertoPS[MAX_PORT];
    sprintf(puertoPS,"%d",ps->numbPort);

    printf("Intentando conectar al Payment Server...\n");
    if((*psSocket=connectTCP(ps->dirIP, puertoPS))==CONNECT_ERROR)
    {
      fprintf(stderr,"Error al intentar conectar al Payment Server.\n\n");
      return -1;
    }

    printf("Payement Server Conectado con exito\n\n");
    return 0;
}


void
enterPSRequest(psMsgRequestT * msg, int fd)
{
  char buffer[MAX_BUFFER_REQUEST]={0};
  
  char amount[MAX_AMOUNT]={0};
  char securityCode[MAX_SEC_CODE]={0};

  writeBuffer(buffer,PS_REQ_HEADER,"");
  writeBuffer(buffer,PS_REQ_CLTSRV,msg->clientServer);
  writeBuffer(buffer,PS_REQ_A_NAME,msg->accountName);
  writeBuffer(buffer,PS_REQ_A_NUM,msg->accountNumber);

  sprintf(securityCode,"%d",msg->securityCode);

  writeBuffer(buffer,PS_REQ_SEC_CODE,securityCode);  

  sprintf(amount,"%.2f",msg->amount);

  writeBuffer(buffer,PS_REQ_AMOUNT,amount);
  
  writeOnSock(fd,buffer,strlen(buffer)+1);

}

int
getPSRequest(psMsgRequestT * msg, int fd)
{
  char buffer[MAX_BUFFER_REQUEST]={0};
  char *line;

  readTextFromSock(fd,buffer,MAX_REQ_LINES,MAX_BUFFER_REQUEST);


  if ((line=strtok(buffer,"\n"))==NULL || readBuffer(line,PS_REQ_HEADER,NULL,0)==READ_ERROR)
      return READ_ERROR;

  
  if ((line=strtok(NULL,"\n"))==NULL || readBuffer(line,PS_REQ_CLTSRV,msg->clientServer,MAX_CLT_SRV)==READ_ERROR)
	return READ_ERROR;


  if ((line=strtok(NULL,"\n"))==NULL || readBuffer(line,PS_REQ_A_NAME,msg->accountName,MAX_A_NAME)==READ_ERROR)
	return READ_ERROR;      


  if ((line=strtok(NULL,"\n"))==NULL || readBuffer(line,PS_REQ_A_NUM,msg->accountNumber,MAX_A_NUM)==READ_ERROR)
	return READ_ERROR;      


  char securityCode[MAX_SEC_CODE]={0};

  if ((line=strtok(NULL,"\n"))==NULL || readBuffer(line,PS_REQ_SEC_CODE,securityCode,MAX_SEC_CODE)==READ_ERROR)
	return READ_ERROR;      

  msg->securityCode=atoi(securityCode);

  char amount[MAX_AMOUNT]={0};
 
  if ((line=strtok(NULL,"\n"))==NULL || readBuffer(line,PS_REQ_AMOUNT,amount,MAX_AMOUNT)==READ_ERROR)
	return READ_ERROR;

  msg->amount=atof(amount);
  
  return READ_OK;
}

void
enterPSReply(psMsgReplyT * msg, int fd)
{
  char buffer[MAX_BUFFER_REPLY]={0};
  
  char statusCode[MAX_STATUS_CODE]={0};
  sprintf(statusCode,"%d",msg->statusCode);

  writeBuffer(buffer,PS_REPLY_HEADER,statusCode);

  if (msg->statusCode==0)
      writeBuffer(buffer,PS_REPLY_TRANSACTION,msg->value);
  else
      writeBuffer(buffer,PS_REPLY_REASON,msg->value);

  writeOnSock(fd,buffer,strlen(buffer)+1);
}

int
getPSReply(psMsgReplyT * msg, int fd)
{
  char buffer[MAX_BUFFER_REPLY]={0};
  char *line;
  char statusCode[MAX_STATUS_CODE]={0};

  readTextFromSock(fd,buffer,MAX_REPLY_LINES,MAX_BUFFER_REPLY);

  if ((line=strtok(buffer,"\n"))==NULL || readBuffer(line,PS_REPLY_HEADER,statusCode,MAX_STATUS_CODE)==READ_ERROR)
      return READ_ERROR;


  msg->statusCode=atoi(statusCode);

  if (msg->statusCode<0 || msg->statusCode>3)
  {
    fprintf(stderr,"Error en el status code\n");
    return READ_ERROR;
  }


  if ((line=strtok(NULL,"\n"))==NULL )
      return READ_ERROR;

  if (msg->statusCode==0)
  {
	if (readBuffer(line,PS_REPLY_TRANSACTION,msg->value,MAX_REPLY_VALUE)==READ_ERROR)
	    return READ_ERROR;
  }
  else
  {
	if (readBuffer(line,PS_REPLY_REASON,msg->value,MAX_REPLY_VALUE)==READ_ERROR)
	    return READ_ERROR;
  }

  return READ_OK;
}







