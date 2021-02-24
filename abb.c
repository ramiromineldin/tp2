#include "abb.h"
#include "abb.h"
#include "pila.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct nodo_abb {
    struct nodo_abb* izq; 
    struct nodo_abb* der; 
    char* clave; 
    void* dato; 
} nodo_abb_t;

typedef int (*abb_comparar_clave_t) (const char *, const char *);
typedef void (*abb_destruir_dato_t) (void *);
struct abb {
    nodo_abb_t* raiz; 
    size_t cant;
    abb_destruir_dato_t destruir; 
    abb_comparar_clave_t comparar;
};

struct abb_iter {
    pila_t* pila; 
};


typedef struct abb abb_t;
/* *****************************************************************
 *                     FUNCIONES AUXILIARES                        *
 * *****************************************************************/

nodo_abb_t* nodo_abb_crear(const char *clave, void *dato) {
    nodo_abb_t *nodo = malloc(sizeof(nodo_abb_t));
    if (!nodo) return NULL;

    char *clave_a_guardar = strdup(clave);
    if (!clave_a_guardar) {
        free(nodo);
        return NULL;
    }
    nodo->izq  = NULL;
    nodo->der = NULL;
    nodo->clave = clave_a_guardar;
    nodo->dato = dato;
    return nodo;   
}
void nodo_abb_destruir(nodo_abb_t *nodo) {
    free(nodo->clave);
    free(nodo);
}

nodo_abb_t* buscar_nodo(abb_comparar_clave_t comparar, nodo_abb_t *nodo, const char *clave) {
    if(!nodo) return NULL;
    if (comparar(nodo->clave, clave) == 0) return nodo;
    else if (comparar(nodo->clave, clave) > 0) return buscar_nodo(comparar, nodo->izq, clave);
    return buscar_nodo(comparar, nodo->der, clave);
}

nodo_abb_t* wrp_buscar_nodo(abb_comparar_clave_t comparar, nodo_abb_t *nodo, nodo_abb_t *padre, const char *clave) {
    if (!nodo) return padre;
    padre = nodo;
    if (comparar(nodo->clave, clave) == 0) return nodo;
    else if (comparar(nodo->clave, clave) > 0) return wrp_buscar_nodo(comparar, nodo->izq, padre, clave);
    return wrp_buscar_nodo(comparar, nodo->der, padre, clave);
}

nodo_abb_t* buscar_nodo_volumen_2(abb_t* arbol, const char *clave) {
    if (arbol->comparar(arbol->raiz->clave, clave) == 0) return arbol->raiz;
    if (arbol->comparar(arbol->raiz->clave, clave) > 0) return wrp_buscar_nodo(arbol->comparar, arbol->raiz->izq, arbol->raiz, clave);
    return wrp_buscar_nodo(arbol->comparar, arbol->raiz->der, arbol->raiz, clave);
}

nodo_abb_t* wrp_buscar_primer_nodo(nodo_abb_t* nodo) {
    if (nodo->izq == NULL) return nodo;
    return wrp_buscar_primer_nodo(nodo->izq);
}
nodo_abb_t* buscar_primer_nodo(abb_t* abb) {
    return wrp_buscar_primer_nodo(abb->raiz);
}

nodo_abb_t* buscar_padre(nodo_abb_t *nodo, const char *clave, abb_comparar_clave_t comparar) {
    if (comparar(nodo->clave, clave) == 0) {
     return NULL;
    }
    
    if (comparar(nodo->clave, clave) > 0) {
        if (!nodo->izq || comparar(nodo->izq->clave, clave) == 0) return nodo;
        
        return buscar_padre(nodo->izq, clave, comparar);
    }
        
    else if (comparar(nodo->clave, clave) < 0) {
        if (!nodo->der || comparar(nodo->der->clave, clave) == 0) return nodo;
    }
    return buscar_padre(nodo->der, clave, comparar);
}
int cantidad_hijos(nodo_abb_t *nodo) {
    if (nodo->izq && nodo->der) return 2;
    if (!nodo->izq && !nodo->der) return 0;
    return 1;
}


nodo_abb_t* buscar_sucesor(nodo_abb_t *nodo) {
    nodo_abb_t *nodo_actual = nodo;
    while (nodo_actual->der) {
        nodo_actual = nodo_actual->der;
    }
    return nodo_actual;
}
void nodo_borrar_sin_hijos(abb_t *arbol, nodo_abb_t *nodo, nodo_abb_t *padre) {
    if (!padre) {
        arbol->raiz = NULL;
    }
    else if (padre->izq && arbol->comparar(padre->izq->clave, nodo->clave) == 0) {
        padre->izq = NULL;
    }
    else {
        padre->der = NULL;
    }
    nodo_abb_destruir(nodo);
    arbol->cant--;
}

void nodo_borrar_un_hijo(abb_t *arbol, nodo_abb_t *nodo, nodo_abb_t *padre) {
    if (nodo->izq) {
        if (!padre) arbol->raiz = nodo->izq;
        else if (padre->izq && arbol->comparar(padre->izq->clave, nodo->clave) == 0) padre->izq = nodo->izq;
        else {
            padre->der = nodo->izq;
        }
    }
    else {
        if (!padre) arbol->raiz = nodo->der;
        else if (padre->izq && arbol->comparar(padre->izq->clave, nodo->clave) == 0) padre->izq = nodo->der;
        else {
            padre->der = nodo->der;
        }
    }
    nodo_abb_destruir(nodo);
    arbol->cant--;
}

void nodo_borrar_dos_hijos(abb_t* arbol, nodo_abb_t *nodo) {
    nodo_abb_t *nodo_sucesor = buscar_sucesor(nodo->izq);
    char *clave_sucesor = strdup(nodo_sucesor->clave);
    void* dato_sucesor = nodo_sucesor->dato;
    int nro_hijos_sucesor = cantidad_hijos(nodo_sucesor);
    if (nro_hijos_sucesor == 0) {
        nodo_abb_t *padre = buscar_padre(arbol->raiz, nodo_sucesor->clave, arbol->comparar);
        nodo_borrar_sin_hijos(arbol, nodo_sucesor, padre);
        free(nodo->clave);
        nodo->dato = dato_sucesor;
        nodo->clave = clave_sucesor;
    }
    else {
        nodo_abb_t *padre = buscar_padre(arbol->raiz, nodo_sucesor->clave, arbol->comparar);
        nodo_borrar_un_hijo(arbol, nodo_sucesor, padre);
        free(nodo->clave);
        nodo->dato = dato_sucesor;
        nodo->clave = clave_sucesor;
    }
}

void wp_abb_destruir(nodo_abb_t *nodo, abb_destruir_dato_t destruir) {
    if (!nodo) return;
    wp_abb_destruir(nodo->izq, destruir);
    wp_abb_destruir(nodo->der, destruir);
    void *dato = nodo->dato;
    nodo_abb_destruir(nodo);
    if (destruir) {
        destruir(dato);
    }
}

/* *****************************************************************
 *                     PRIMITIVAS DEL ABB                          *
 * *****************************************************************/


abb_t* abb_crear(abb_comparar_clave_t cmp, abb_destruir_dato_t destruir_dato) {
    abb_t *abb = malloc(sizeof(abb_t));
    if (!abb) return NULL;

    abb->raiz = NULL;
    abb->cant = 0;
    abb->destruir = destruir_dato;
    abb->comparar = cmp;
    return abb;
}

bool abb_guardar(abb_t *arbol, const char *clave, void *dato) {
    if (!arbol) return false;
    if (arbol->cant == 0) {
        nodo_abb_t *nueva_raiz = nodo_abb_crear(clave, dato);
        arbol->raiz = nueva_raiz;
        arbol->cant++;
        return true;
    }
    else if (arbol->comparar(arbol->raiz->clave, clave) == 0) {
        if (arbol->destruir) arbol->destruir(arbol->raiz->dato);
        arbol->raiz->dato = dato;
        return true;
    }
    nodo_abb_t *padre = buscar_padre(arbol->raiz, clave, arbol->comparar);
    if (padre->der && arbol->comparar(padre->der->clave, clave) == 0) {
        if (arbol->destruir) arbol->destruir(padre->der->dato);
        padre->der->dato = dato;
        return true;
    }
    else if (padre->izq && arbol->comparar(padre->izq->clave, clave) == 0) {
        if (arbol->destruir) arbol->destruir(padre->izq->dato);
        padre->izq->dato = dato;
        return true;
    }
    nodo_abb_t *nodo_nuevo = nodo_abb_crear(clave, dato);
    if (arbol->comparar(padre->clave, clave) > 0) {
        padre->izq = nodo_nuevo;
    }
    else {
        padre->der = nodo_nuevo;
    }
    arbol->cant++;
    return true;
}

void *abb_borrar(abb_t *arbol, const char *clave) {
    if (!arbol || arbol->cant == 0) return NULL;
    nodo_abb_t *nodo_borrar = buscar_nodo(arbol->comparar, arbol->raiz, clave); 
    if (!nodo_borrar) return NULL;
    void *dato = nodo_borrar->dato;
    int nro_hijos = cantidad_hijos(nodo_borrar);
    if (nro_hijos == 0) {
        nodo_abb_t *padre = buscar_padre(arbol->raiz, clave, arbol->comparar);
        nodo_borrar_sin_hijos(arbol, nodo_borrar, padre);
    }
    else if (nro_hijos == 1) {
        nodo_abb_t *padre = buscar_padre(arbol->raiz, clave, arbol->comparar);
        nodo_borrar_un_hijo(arbol, nodo_borrar, padre);
    }
    else {
        nodo_borrar_dos_hijos(arbol, nodo_borrar);
    }
    return dato;
}

void *abb_obtener(const abb_t *arbol, const char *clave) {
    nodo_abb_t *nodo = buscar_nodo(arbol->comparar, arbol->raiz, clave);
    if (!nodo) return NULL;
    return nodo->dato;  
}

bool abb_pertenece(const abb_t *arbol, const char *clave) {
    nodo_abb_t *nodo = buscar_nodo(arbol->comparar, arbol->raiz, clave);
    if (!nodo) return false;
    return true;
}

size_t abb_cantidad(abb_t *arbol) {
    return arbol->cant;
}

void abb_destruir(abb_t *arbol) {
    wp_abb_destruir(arbol->raiz, arbol->destruir);
    free(arbol);
}

/* *****************************************************************
 *                    PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/
void abb_iterar(nodo_abb_t* nodo, bool visitar(const char*, void*, void *, size_t*), void *extra, bool* stop, void* contador) {
    if (!nodo || !(*stop) || strcmp(nodo->clave, (char*) extra_iniciar) < 0) {
        return;
    }
    abb_iterar(nodo->izq, visitar, extra, stop, contador);
    if (!(*stop) || !(visitar(nodo->clave, nodo->dato, extra, contador))) {
        *stop = false;
        return;
    }
    abb_iterar(nodo->der, visitar, extra, stop, contador);
}
void abb_in_order(abb_t *arbol, bool visitar(const char *, void  *, void *, size_t*), void *extra_empezar, void *extra_terminar, size_t* contador) {
    bool stop = true;
    nodo_abb_t* nodo_aux;
    if (strcmp(extra_empezar, "") == 0) {
       nodo_aux = buscar_primer_nodo(arbol);
    }
    else {
        nodo_aux = buscar_nodo_volumen_2(arbol, extra_empezar); 
         if (strcmp(nodo_aux->clave, extra_empezar) < 0) {
            nodo_aux = NULL;
        }
    }
    abb_iterar(nodo_aux, visitar, extra_terminar, &stop, contador);
}
/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

abb_iter_t *abb_iter_in_crear(const abb_t *arbol) {
    abb_iter_t* iter = malloc(sizeof(abb_iter_t));
    if (!iter) {
        return NULL;
    }
    if (!(iter->pila = pila_crear())) {
        free(iter); 
        return NULL;
    }
    if (arbol->raiz) {
        pila_apilar(iter->pila, arbol->raiz);
        nodo_abb_t* nodo_aux = arbol->raiz->izq;
        while(nodo_aux) {
            pila_apilar(iter->pila, nodo_aux); 
            nodo_aux = nodo_aux->izq;
        }
    }
    return iter;
}
bool abb_iter_in_al_final(const abb_iter_t *iter) {
    return pila_esta_vacia(iter->pila);
}

bool abb_iter_in_avanzar(abb_iter_t *iter) {
    if (abb_iter_in_al_final(iter)) {
        return false;
    }
    nodo_abb_t *nodo_aux_desapilado = pila_desapilar(iter->pila); 
    if (nodo_aux_desapilado->der) {
        nodo_abb_t *nodo_aux  = nodo_aux_desapilado->der;
        while(nodo_aux) {
            pila_apilar(iter->pila, nodo_aux);
            nodo_aux = nodo_aux->izq;
        }
    }
    return true;
}

const char *abb_iter_in_ver_actual(const abb_iter_t *iter) {
    if(abb_iter_in_al_final(iter)) {
        return NULL;
    }
    nodo_abb_t* nodo_aux = pila_ver_tope(iter->pila);
    return nodo_aux->clave;
}

void abb_iter_in_destruir(abb_iter_t* iter) {
    pila_destruir(iter->pila);
    free(iter);
}

