#ifndef _MSG_P_SERVER_H
#define _MSG_P_SERVER_H


#include "connect.h"

#include "msgsock.h"
#include <stdio.h>
#include <string.h>

#define MAX_PORT 10

#define ISERVER_ERROR -1


#define PS_REQ_CLTSRV "ClientServer: "
#define PS_REQ_A_NAME "AccountName: "
#define PS_REQ_A_NUM "AccountNumber: "
#define PS_REQ_SEC_CODE "SecurityCode: "
#define PS_REQ_AMOUNT "Amount: "

#define MAX_REQ_LINES 6

#define MAX_CLT_SRV 25
#define MAX_A_NAME 64
#define MAX_A_NUM 64
#define MAX_SEC_CODE 15
#define MAX_AMOUNT 15

#define MAX_BUFFER_REQUEST 256

#define PS_REQ_HEADER "CHARGE PaymentService/1.0"

typedef struct{
    char clientServer[MAX_CLT_SRV+1];
    char accountName[MAX_A_NAME+1];
    char accountNumber[MAX_A_NUM+1];
    int securityCode;
    double amount;
}psMsgRequestT;



#define STATUS_OK 0
#define STATUS_INVALID_REQUEST 1
#define STATUS_INVALID_COUNT 2
#define STATUS_NOMONEY 3

#define MAX_REPLY_LINES 2

#define PS_REPLY_HEADER "PaymentService/1.0 "
#define MAX_REPLY_VALUE 256

#define PS_REPLY_TRANSACTION "Transaction: "
#define PS_REPLY_REASON "Reason: "

#define MAX_BUFFER_REPLY 300

#define MAX_STATUS_CODE 2

typedef struct{
    int statusCode;
    char value[MAX_REPLY_VALUE];
}psMsgReplyT;


int connectToPS(endPoint *ps, int *psSocket);

void enterPSRequest(psMsgRequestT * msg, int fd);
int getPSRequest(psMsgRequestT * msg, int fd);

void enterPSReply(psMsgReplyT * msg, int fd);
int getPSReply(psMsgReplyT * msg, int fd);






#endif

