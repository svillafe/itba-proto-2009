#include "../include/listGen.h"

typedef struct nodoCDT
{
	listElementT dato;
	struct nodoCDT * tail;
} nodoCDT;

/* nodo header diferenciado */
struct listCDT
{
	TfuncCompList compara;
	nodoCDT* nodos;
	nodoCDT* actual;
};

/* No confundir esta función con el otro ListTail. Este es static o sea
* no es parte del contrato del TAD, y Segundo se usa sobre la sublista sin
* header.*/
static nodoCDT*
listTail(nodoCDT* lista)
{
	if ( lista == NULL)
		return NULL;
	else
		return lista->tail;
}

listADT
listNew (TfuncCompList compara)
{
	struct listCDT *auxi;
	
	if (compara == NULL)
		return NULL;
	if ( ( auxi = malloc(sizeof(struct listCDT))) == NULL )
		return NULL;
	auxi->compara = compara;
	auxi->nodos = NULL;
	auxi->actual=NULL;
	return auxi;
}

int
listInsert (listADT lista, listElementT elem)
{
	nodoCDT *rec, *ant, *auxi;
	int dif;
	rec = ant = lista->nodos;
	while ( rec != NULL && ( dif = (*lista->compara)(rec->dato, elem)) < 0 )
	{
		ant = rec;
		rec = rec->tail;
	}
	/* No se insertan elementos repetidos */
	if ( rec != NULL && dif == 0 )
		return 0;
	/* Crear espacio para el nuevo nodo */
	if ( ( auxi = malloc(sizeof(*auxi))) == NULL)
		return 0;
	/* Copiar la informacion al nuevo nodo */
	auxi->dato = elem;
	/* Insertar entre ant y rec */
	auxi->tail = rec;
	if ( ant == rec ) /* Inserta al principio */
		lista->nodos = auxi;
	else
		/* Inserta en el medio */
		ant->tail = auxi;
	return 1;
}

int
listElemDelete (listADT list, listElementT elem)
{
	nodoCDT *rec=NULL, *ant=NULL;
	int dif = -1; rec = ant = list->nodos;
	while ( rec != NULL && ( dif = (*list->compara)(rec->dato, elem)) < 0 )
	{
		ant = rec;
		rec = rec->tail;
	}
	if ( dif != 0 )
		return 0;
	/* Encadenar elemento anterior con el siguiente */
	if (ant == rec)
		list->nodos = rec->tail;
	else
		ant->tail = rec->tail;

	/* si el elemento a borrar era el indicado por actual debemos mover actual para que apunte a
	** un elemento dentro de la lista y no uno borrado */
	if (rec == list->actual)
		list->actual= listTail(list->actual);
	/* liberar el nodo */
	free(rec);
	return 1;
}

int
listElemBelong (listADT list, listElementT elem)
{
	struct nodoCDT *auxi;
	int dif; auxi = list->nodos;
	while ( auxi != NULL && ( dif = (*list->compara)(elem,auxi->dato)) > 0 )
		auxi = auxi->tail;
	return ( auxi != NULL && dif == 0 );
}

void
listFree (listADT list)
{
	nodoCDT *rec, *auxi;

	if (list==NULL)  //si la lista esta vacia sale
	    return;

	rec = list->nodos;
	while ( rec != NULL )
	{
		auxi = rec;
		rec = rec->tail;
		/* liberar el nodo */
		free(auxi);	
	}
	free(list);
}

int
listIsEmpty (listADT list)
{
	return (list->nodos == NULL);
}

void
ToBegin(listADT list)
{
	list->actual = list->nodos;
}

listElementT
GetDato(listADT list)
{
	listElementT dato = NULL;

	if (list->actual != NULL)
	{
		dato= list->actual->dato;

		list->actual= listTail(list->actual);
	}

	return dato;
}


