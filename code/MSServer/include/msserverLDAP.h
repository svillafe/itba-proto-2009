#ifndef _MS_SERVER_LDAP_H
#define _MS_SERVER_LDAP_H


#include <stdlib.h>
#include <stdio.h>
#include <ldap.h>

#include "../../Sharelibs/include/opStructs.h"
#include "../../Sharelibs/include/listGen.h"

// Usuario administrador del ldap 
#define ADMIN_DN "cn=admin,dc=MSServer,dc=com"
#define ADMIN_PASSWD "admin"


#define MAX_LDAP_NAME 128
#define MAX_LDAP_LEVEL 15
#define MAX_LDAP_PWD 15
#define PEOPLE_PATH "ou=people,dc=MSServer,dc=com"



int userExists(char *user);

int getUserPswd(char *user,char *pswd);

int getUserLevel(char *user,int *level);

int changeUserPswd(char *user,char *passwd);

listADT getUsers(void);


#endif
