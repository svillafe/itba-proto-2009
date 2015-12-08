# MakeFile de Protocolos de comunicacion
.SILENT:

SRC_DES=des/source

SRC_SHR=Sharelibs/src

SRC_MS_CLT=MSClient/src

SRC_MS_SRV=MSServer/src

SRC_PL_SRV=PLServer/src

SRC_PS_SRV=PServer/src

#Objetos de encriptacion
OBJ_DES=$(SRC_DES)/des.o $(SRC_DES)/desBit.o

#Objetos comunes entre servidor y cliente
OBJ_SHR= $(OBJ_DES) $(SRC_SHR)/slib.o $(SRC_SHR)/connect.o $(SRC_SHR)/passivesock.o $(SRC_SHR)/connectionclose.o $(SRC_SHR)/msgsock.o $(SRC_SHR)/encriptar.o $(SRC_SHR)/theReaders.o $(SRC_SHR)/magicCookie.o $(SRC_SHR)/parse.o $(SRC_SHR)/listGen.o $(SRC_SHR)/md5.o $(SRC_SHR)/msgPaymentServer.o $(SRC_SHR)/msgPLServer.o 

#Objetos del Movie Store Client
OBJ_MS_CLT=$(OBJ_SHR) $(SRC_MS_CLT)/msclient.o  $(SRC_MS_CLT)/msclientConnect.o  $(SRC_MS_CLT)/msclientBuy.o $(SRC_MS_CLT)/msclientHelp.o $(SRC_MS_CLT)/msclientDownload.o $(SRC_MS_CLT)/msclientChgPass.o $(SRC_MS_CLT)/msclientListClients.o $(SRC_MS_CLT)/msclientListMovies.o $(SRC_MS_CLT)/msclientListGen.o $(SRC_MS_CLT)/msclientDisconnect.o 


#Objetos del Movie Store Server
OBJ_MS_SRV= $(OBJ_SHR)  $(SRC_MS_SRV)/msserver.o $(SRC_MS_SRV)/msserverLib.o $(SRC_MS_SRV)/msserverDownload.o $(SRC_MS_SRV)/msserverBuyMovie.o $(SRC_MS_SRV)/msserverConnectUser.o $(SRC_MS_SRV)/msserverChgPass.o $(SRC_MS_SRV)/msserverListClients.o $(SRC_MS_SRV)/msserverListMovies.o $(SRC_MS_SRV)/msserverListGen.o $(SRC_MS_SRV)/msserverManageFile.o $(SRC_MS_SRV)/msserverDisconnectUser.o $(SRC_MS_SRV)/msserverLDAP.o $(SRC_MS_SRV)/msserverTTL.o

#Objetos del Payment lookup Server.
OBJ_PL_SRV=$(OBJ_SHR) $(SRC_PL_SRV)/plserver.o $(SRC_PL_SRV)/plserverLib.o

#Objetos del Payment Server.
OBJ_PS_SRV=$(OBJ_SHR) $(SRC_PS_SRV)/pserver.o $(SRC_PS_SRV)/pserverLib.o



EJEC_MS_CLT=msclient

EJEC_MS_SRV=msserver

EJEC_PL_SRV=plserver

EJEC_PS_SRV=pserver



CC = cc
CFLAGS= -Wall -ansi -uninitialized -pedantic -lrt -g -lm -lssl -lldap
COPTS = -c
LD = $(CC)
LDOPTS = -o


all: $(EJEC_PL_SRV) $(EJEC_MS_SRV) $(EJEC_MS_CLT) $(EJEC_PS_SRV)

$(EJEC_PL_SRV): $(OBJ_PL_SRV)
	@echo
	@echo Obteniendo ejecutable $(EJEC_PL_SRV)
	@echo
	$(LD) $(CFLAGS) $(LDOPTS) $(EJEC_PL_SRV) $(OBJ_PL_SRV)

$(EJEC_MS_SRV): $(OBJ_MS_SRV)
	@echo
	@echo Obteniendo ejecutable $(EJEC_MS_SRV)
	@echo
	$(LD) $(CFLAGS) $(LDOPTS) $(EJEC_MS_SRV) $(OBJ_MS_SRV)


$(EJEC_MS_CLT): $(OBJ_MS_CLT)
	@echo
	@echo Obteniendo ejecutable $(EJEC_MS_CLT)
	@echo
	$(LD) $(CFLAGS) $(LDOPTS) $(EJEC_MS_CLT) $(OBJ_MS_CLT)



$(EJEC_PS_SRV): $(OBJ_PS_SRV)
	@echo
	@echo Obteniendo ejecutable $(EJEC_PS_SRV)
	@echo
	$(LD) $(CFLAGS) $(LDOPTS) $(EJEC_PS_SRV) $(OBJ_PS_SRV)

.c.o:
	echo compilando $<
	$(CC) $(COPTS) $<
	if [ -f plserver*.o ]; then mv plserver*.o $(SRC_PL_SRV); fi;
	if [ -f msclient*.o ]; then mv msclient*.o $(SRC_MS_CLT); fi;
	if [ -f msserver*.o ]; then mv msserver*.o $(SRC_MS_SRV); fi;
	if [ -f pserver*.o  ]; then mv pserver*.o  $(SRC_PS_SRV); fi;
	if [ -f des*.o ]; then mv *.o $(SRC_DES); fi;
	if [ -f *.o ]; then mv *.o $(SRC_SHR); fi;
	
	
clear:
	echo
	echo Limpiando directorio
	echo

	#Borra objetos
	rm -f $(SRC_DES)/*.o
	rm -f $(SRC_SHR)/*.o 
	rm -f $(SRC_MS_SRV)/*.o 
	rm -f $(SRC_MS_CLT)/*.o
	rm -f $(SRC_PL_SRV)/*.o 
	rm -f $(SRC_PS_CLT)/*.o

	#Borra archivos temporales
	rm -f $(SRC_DES)/*~
	rm -f $(SRC_SHR)/*~
	rm -f $(SRC_MS_SRV)/*~
	rm -f $(SRC_MS_CLT)/*~
	rm -f $(SRC_PL_SRV)/*~
	rm -f $(SRC_PS_SRV)/*~

	#Borra ejecutables	
	rm -f msserver*
	rm -f msclient*
	rm -f plserver*
	rm -f pserver*
	rm -f *~
again: 
	make clear
	make all
