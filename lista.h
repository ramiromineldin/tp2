#ifndef LISTA_H
#define LISTA_H

#include <stdbool.h>
#include <stddef.h>

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 * *****************************************************************/

/* Se trata de una lista que contiene datos de tipo void* (punteros genericos) */

typedef struct lista lista_t;

typedef struct lista_iter lista_iter_t;

/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA                        *
 * ******************************************************************/

// Crea una lista.
// Post : devuelve una nueva lista vacia. En caso de error devuelve NULL.
lista_t *lista_crear(void);

// Destruye la lista. Si recibe la funcion destruir_dato por parametro.
// Llama a destruir_dato para cada uno de los elementos.
// Pre: la lista fue creada, destruir_dato es una funcion que destruye los datos de la lista o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

// Devuelve verdadero si la lista esta vacia, en caso contrario devuelve falso.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

// Agrega un nuevo elemento en la primer posicion de la lista. En caso de error devuelve falso.
// Pre: la lista fue creada.
// Post: se agrego un nuevo elemento al principio de la lista.
bool lista_insertar_primero(lista_t *lista, void *dato);

// Agrega un nuevo elemento en la ultima posicion de la lista. En caso de error devuelve falso.
// Pre: la lista fue creada.
// Post: se agrego un nuevo elemento al final de la lista.
bool lista_insertar_ultimo(lista_t *lista, void *dato);

// Saca el primer elemento de la lista. Si la lista tiene elementos, se saca al primero y devuelve su valor, si esta vacia devuelve NULL.
// Pre: la lista fue creada
// Post: Si la lista no estaba vacia, se devuelve el valor del primer elemento anterior, y lista tiene un elemento menos, en caso contrario se devolvió NULL.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista, si la lista tiene elementos se devuelve el valor del primer elemento. Si esta vacia devuelve NULL.
// Pre: la lista fue creada
// Post: si la lista no estaba vacia, se devolvió el valor del primer elemento de la lista,  NULL en caso contrario.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del ultimo elemento de la lista, si la lista tiene elementos se devuelve el valor del ultimo elemento. Si esta vacia devuelve NULL.
// Pre: la lista fue creada
// Post: si la lista no estaba vacia, se devolvió el valor del ultimo elemento de la lista, en caso contrario se devolvió NULL.
void *lista_ver_ultimo(const lista_t* lista);

// Devuelve el largo de la lista.
// Pre: la lista fue creada. 
size_t lista_largo(const lista_t *lista);

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR INTERNO               *
 * ******************************************************************/

// Itera la lista que se pasa por parametro y le aplica la funcion visitar a cada elemento. 
// El parametro extra es para establecer una condicion de corte. 
// En caso de no utilizar una condicion de corte pasar NULL como parametro.
// Post: se le aplico la funcion visitar a cada elemento de la lista.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);


/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO               *
 * ******************************************************************/

// Crea el iterador
// Post : devuelve el iterador. En caso de error devuelve NULL.
lista_iter_t *lista_iter_crear(lista_t *lista);

// Avanza el iterador una posicion. Devuelve falso en caso de error.
// Pre: el iterador fue creado
bool lista_iter_avanzar(lista_iter_t *iter);

// Devuelve el dato al que esta apuntando el iterador.
// Pre: el iterador fue creado.
// Post: se devolvio el dato actual.
void *lista_iter_ver_actual(const lista_iter_t *iter);

// Devuelve verdadero si el iterador apunta al final de la lista. Falso en caso contrario.
// Pre: el iterador fue creado.
bool lista_iter_al_final(const lista_iter_t *iter);

// Destruye el iterador.
// Pre: el iterador fue creado.
void lista_iter_destruir(lista_iter_t *iter);

// Inserta un elemento en donde esta apuntando el iterador y devuelve verdadero. En caso de error devuelve falso.
// Pre: el iterador fue creado.
// Post: se inserto  un elemento en la lista.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

// Elimina el elemento al que esta apuntando el iterador y devuelve su valor. En caso de error devuelve NULL.
// Pre: el iterador fue creado.
// Post: se elimino un elemento de la lista y se devolvio su valor.
void *lista_iter_borrar(lista_iter_t *iter);

#endif // LISTA_H