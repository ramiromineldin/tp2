#ifndef ABB_H
#define ABB_H
#include <stdbool.h>
#include <stddef.h>

/* *****************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS                 *
 * *****************************************************************/

typedef struct abb abb_t;
typedef struct abb_iter abb_iter_t;
typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);

/* *****************************************************************
 *                     PRIMITIVAS DEL ABB                          *
 * *****************************************************************/

// Crea el arbol binario de busqueda. En caso de error devuelve NULL.
// Recibe por parametro una funcion de comparacion y una de destruccion.
// En caso de que no se utilice la funcion de destruccion, pasar NULL como parametro
// Post: Devuelve un arbol binario de busqueda vacio.
abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato);

// Guarda la clave con el dato asociado.
// Pre: el arbol fue creado
// Post: Se guardo una nueva clave con el dato asociado en el arbol y devuelve True. En caso de error devuelve False.
bool abb_guardar(abb_t *arbol, const char *clave, void *dato);

// Borra la clave del arbol y devuelve el dato asociado a ella
// Pre: El arbol fue creado.
// Post: Se borro la clave del arbol. Si la clave no pertenece al arbol devuelve NULL
void *abb_borrar(abb_t *arbol, const char *clave);

// Obtiene el dato asociado a la clave y lo devuelve. 
// Pre: El arbol fue creado.
// Post: El dato fue devuelto. En caso de que la clave no pertenezca al arbol devuelve NULL
void *abb_obtener(const abb_t *arbol, const char *clave);

// Comprueba si una clave pertenece al arbol.
// Pre: El arbol fue creado.
// Post: Devuelve true si la clave pertence al arbol o False en caso contrario.
bool abb_pertenece(const abb_t *arbol, const char *clave);

// Devuelve la cantidad de elementos que hay en el arbol.
// Pre: El arbol fue creado.
// Post: Devuelve la cantidad de elementos.
size_t abb_cantidad(abb_t *arbol);

// Destruye el arbol.
// Pre: El arbol fue creado.
// Post: El arbol fue destruido y se libera la memoria.
void abb_destruir(abb_t *arbol);

/* *****************************************************************
 *                    PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/
//Itera la lista. 
//Se recibe la funcion que se debe aplicar como parametro.
//Post: Se itero la lista y devolvio lo que la funcion visitar solicitaba. 
void abb_in_order(abb_t *arbol, bool visitar(const char *, void *, void *, size_t *),void *extra, void *extra2, size_t* extra3);

/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/
//Crea un iterado, en caso de error devuelve NULL. 
//Recibe un arbol binario como parametro.
//Post: Creo un iterador
abb_iter_t *abb_iter_in_crear(const abb_t *arbol);

//El iterador avanza un nodo en el arbol binario. Si el iterador se encuentra al final retorna false. 
//Pre: El iterador fue creado.
//Post: Avanzo un nodo del arbol binario.
bool abb_iter_in_avanzar(abb_iter_t *iter);

// Obtiene la clave del nodo del arbol binario donde se encuentra el iterador, devuelve NULL
// en caso de estar al final del arbol. 
// Pre: El iterador fue creado.
// Post: se devolvió la clave del nodo donde se encuentra el iterador o NULL si el iterador esta al final.  
const char *abb_iter_in_ver_actual(const abb_iter_t *iter);

//Devuelve true si el iterador se encuentra al final del arbol binario, false en caso contrario.
//Pre: El iterador fue creado. 
//Post: Se devolvio true si el iterador se encontraba al final de la lista, false en caso contrario. 
bool abb_iter_in_al_final(const abb_iter_t *iter);

//Destruye el iterador
//Pre: El iterador fue creado.
//Post: Se destruyo el iterador. 
void abb_iter_in_destruir(abb_iter_t* iter);

#endif  // ABB_H
