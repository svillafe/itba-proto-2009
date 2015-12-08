#include "../include/msserverLDAP.h"

#include <ldap.h>

extern endPoint infoLdap;

static LDAP *
connectLdap(void)
{
	LDAP *ld;
	
	ld = (LDAP *)ldap_open(infoLdap.dirIP,infoLdap.numbPort);

	if( ld == NULL )
	{
		fprintf(stderr,"Error: no se pudo establer comunicacion con LDAP\n");
		return NULL;
	}
	
	//se conecta como administrador
	if( ldap_bind_s(ld,ADMIN_DN,ADMIN_PASSWD,LDAP_AUTH_SIMPLE) != LDAP_SUCCESS )
	{
		fprintf(stderr,"Error: no se pudo conectar como administrador\n");
		return NULL;
	}

	return ld;
}

static void 
endLdap(LDAP *ld)
{
	ldap_unbind(ld);
}


/*Dada un LDAPMessage que corresponde a una entrada del ldap, retorna en client y passwd sus atributos*/

static void
getAttributes(LDAP *ld,LDAPMessage *e,clientT *client,char *passwd)
{
	BerElement *ber;
	char *a;
	char **vals;

	for ( a = ldap_first_attribute( ld, e, &ber ); a != NULL; a = ldap_next_attribute( ld, e, ber ) )
	{
		if ( (vals = (char**)ldap_get_values( ld, e, a)) != NULL )
		{
			if (strcmp("userPassword",a)==0)
				strcpy(passwd,vals[0]);
			else if (strcmp("cn",a)==0)
				strcpy(client->name,vals[0]);
			
			else if (strcmp("mail",a)==0)
				strcpy(client->mail,vals[0]);

			else if (strcmp("description",a)==0)
			{
				client->longDescription=strlen(vals[0])+1;
				client->description=malloc(strlen(vals[0])+1);
				strcpy(client->description,vals[0]);
			}
			else if (strcmp("telexNumber",a)==0)
				client->level=atoi(vals[0]);
			
			
			ldap_value_free( vals );
		}
		ldap_memfree( a );
	}
	
	if ( ber != NULL )
	{
		ber_free( ber, 0 );
	}

}

/*Busca la entrada que corresponde a user. Si user es NULL, retorna todas las entradas de usuarios*/

static LDAPMessage *
search(LDAP *ld,char *user)
{
	int rc;

	LDAPMessage *result;

	char domainName[MAX_LDAP_NAME]={0};

	if (user!=NULL)
		sprintf(domainName,"cn=%s,%s",user,PEOPLE_PATH);
	else
		sprintf(domainName,"%s",PEOPLE_PATH);

	if ( ( rc = ldap_search_ext_s( ld, domainName, LDAP_SCOPE_SUBTREE,"(cn=*)", NULL, 0, NULL, NULL, LDAP_NO_LIMIT,LDAP_NO_LIMIT, &result ) ) != LDAP_SUCCESS )
	{
		return NULL; //no encuentra nada
	}
	return result;
}


int
userExists(char *user)
{
	LDAP *ld;
	LDAPMessage * result;
	int res;

	if ( (ld=connectLdap())==NULL)
		return 0;
	
	res= (result=search(ld,user))==NULL?0:1;

	if (res)
		ldap_msgfree( result );
	
	endLdap(ld);
	
	return res;	
}

int
getUserPswd(char *user,char *passwd)
{
	LDAP *ld;
	clientT client;
	LDAPMessage * result,*e;

	if ( (ld=connectLdap())==NULL)
		return -1;

	if ( (result=search(ld,user))==NULL)
	{
		endLdap(ld);
		return -1;
	}

	if ( (e = ldap_first_entry( ld, result ))==NULL)
	{
		ldap_msgfree( result );
		endLdap(ld);
		return -1;
	}

	getAttributes(ld,e,&client,passwd);

	ldap_msgfree( result );
	endLdap(ld);

	return 0;
}


int
getUserLevel(char *user,int *level)
{
	LDAP *ld;
	clientT client;
	LDAPMessage * result,*e;
	char pwd[MAX_LDAP_PWD];

	if ( (ld=connectLdap())==NULL)
		return -1;

	if ( (result=search(ld,user))==NULL)
	{
		endLdap(ld);
		return -1;
	}

	if ( (e = ldap_first_entry( ld, result ))==NULL)
	{
		ldap_msgfree( result );
		endLdap(ld);
		return -1;
	}

	getAttributes(ld,e,&client,pwd);

	ldap_msgfree( result );
	endLdap(ld);
	*level=client.level;
	return 0;
}


int
changeUserPswd(char *user,char *passwd)
{
	LDAP *ld;

	char domainName[MAX_LDAP_NAME]={0};

	LDAPMod attr;
	
	sprintf(domainName,"cn=%s,%s",user,PEOPLE_PATH);

	if ( (ld=connectLdap())==NULL)
		return -1;

	char *pass[] = {passwd, NULL};
	
	attr.mod_type = "userPassword";
	
	attr.mod_op = LDAP_MOD_REPLACE;
	
	attr.mod_values = pass; 

	LDAPMod *mods[]={&attr,NULL};

	
       	if (ldap_modify_s(ld, domainName, mods) != LDAP_SUCCESS)
	{
		fprintf(stderr,"Error al cambiar la contraseña\n");
		endLdap(ld);
		return -1;
	}

	endLdap(ld);
	return 0;
}

int
compClients(clientT *c1,clientT *c2)
{
	strcmp(c1->name,c2->name);
}

listADT
getUsers(void)
{
	LDAP *ld;
	clientT *client;
	LDAPMessage * result,*e;
	char pwd[MAX_LDAP_PWD];
	listADT listClients=NULL;
	
	if ( (listClients= listNew ( (int(*)(void*,void*))compClients))==NULL)
	{
		fprintf(stderr,"getUseres: Error al iniciar lista");
		return NULL;
	}

	if ( (ld=connectLdap())==NULL)
	{
  		listFree(listClients);
		return NULL;
	}
	if ( (result=search(ld,NULL))==NULL)
	{
		listFree(listClients);
		endLdap(ld);
		return NULL;
	}


	for ( e = ldap_first_entry( ld, result );e!=NULL;e = ldap_next_entry( ld, e ))
	{
		client=malloc(sizeof(clientT));
		getAttributes(ld,e,client,pwd);
		listInsert (listClients, client);
	}

	ldap_msgfree(result);
	endLdap(ld);
	return listClients;
}
