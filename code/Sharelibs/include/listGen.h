/*
* Archivo: listaGen.h
* -------------------------------------------------------------------
* Contrato para un TAD lista que permita manejar datos genericos
* Todos los nodos contendran como informacion un puntero a void.
* El TAD debe recibir, al momento de ser creado:
* -Un puntero a la funcion de comparacion
* -Un puntero a la funcion que copia la informacion
* ( en caso de querer guardar solamente el puntero, enviarle NULL )
* -Un puntero a la funcion que libera la informacion
* ( en caso de no querer liberar la memoria, enviarle NULL )
*/
#ifndef _listADT_h
#define _listADT_h

#include <stdlib.h>

/* tipo de datos de los elementos de la lista */
typedef void * listElementT;
/* tipo de dato para definición de listas */
typedef struct listCDT *listADT;
/* Prototipo de la funcion de comparacion de elementos */
typedef int (* TfuncCompList) (listElementT, listElementT);

/* ------- Operaciones disponibles sobre el TAD ------- */
/* Función: inicializa la lista.
* Recibe como parametro el puntero a la funcion de comparacion
* Uso: conjunto= listNew(compara, copia, libera);
* Crea una nueva instancia de la lista.
* Devuelve NULL en caso de no poder generar una nueva.
*/
listADT listNew (TfuncCompList);
/* Función: agrega un elemento en forma ordenada a la lista.
* Retorna 1 si pudo insertar el elemento y 0 en caso contrario.
*/
int listInsert (listADT lista, listElementT elem);
/* Función: elimina un elemento de la lista.
* Retorna 0 si no pudo borrar el elemento y 1 en caso contrario.
*/
int listElemDelete (listADT lista, listElementT elem);
/* Función: Analiza si un elemento pertenece a la lista.
* Retorna 1 si el elemento esta en la lista y 0 en caso contrario.
*/
int listElemBelong (listADT list, listElementT elem);
/* Funcion: analiza si una lista esta vacio o no
* Retorna 1 si esta vacia y cero si no
*/
int listIsEmpty (listADT lista);
/* Función: Destruye la lista
* Se debe aplicar a toda lista que quede en desuso.
*/
void listFree (listADT lista);
/* Funcion: Operacion que se usa para permitir comenzar a recorrer la
* lista desde el comienzo.
* Uso: ToBegin(lista);
* ---------------------------------------------------------
* Ver la proxima operacion
*/
void ToBegin(listADT lista);
/* Función: Operacion de consulta que sirve para iterar por la lista. La lista sigue intacta.
* Luego de la invocación, el próximo GetDato permitirá acceder al siguiente elemento, y así
* sucesivamente.
* Uso: double *puntero;
* ToBegin( lista);
* while ( (puntero= GetDato( lista)))
* printf("%g\n", *puntero);
* ------------------------------------------------------------------
* Junto con la funcion anterior permite iterar por la lista.
* Devuelve NULL cuando se intenta avanzar y ya esta en el ultimo nodo
*/
listElementT GetDato( listADT lista);

#endif


