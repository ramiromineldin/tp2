#include "lista.h"
#include <stdlib.h>

/* Defincion del struct nodo */
typedef struct nodo {
    void *dato;
    struct nodo *prox;
} nodo_t;

/* Primitivas del nodo */
nodo_t *nodo_crear(void *valor){
    nodo_t *nodo = malloc(sizeof(nodo_t));
    if (!nodo) {
        return NULL;
    }
    nodo->dato = valor;
    nodo->prox = NULL;
    return nodo;
}

void nodo_destruir(nodo_t *nodo) {
    free(nodo);
}

/*----------------------------------------- */

/* Defincion del struct lista */
struct lista {
    nodo_t *prim;
    nodo_t *ult;
    size_t largo;
};
/* Primitivas de la lista */

lista_t *lista_crear(void) {
    lista_t *lista = malloc(sizeof(lista_t));
    if (!lista) return NULL;
    
    lista->prim = NULL;
    lista->ult = NULL;
    lista->largo = 0;
    return lista;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)) {
    while(!lista_esta_vacia(lista)) {
        void* dato = lista_borrar_primero(lista);
        if (destruir_dato != NULL) destruir_dato(dato);
    }
    free(lista);
}

bool lista_esta_vacia(const lista_t *lista) {
    return lista_largo(lista) == 0;
}

bool lista_insertar_primero(lista_t *lista, void *dato) {
    if (!lista) return false;
    
    nodo_t *nodo = nodo_crear(dato);
    if(nodo == NULL) return false;

    if(lista_esta_vacia(lista)) {
        lista->ult = nodo;
    }
    else {
         nodo->prox = lista->prim;
    }

    lista->prim = nodo;
    lista->largo++;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
    if (!lista) return false;
    
    nodo_t *nodo = nodo_crear(dato);
    if(nodo == NULL) return false;

    if (lista_esta_vacia(lista)) {
        lista->prim = nodo;
    }
    else {
        lista->ult->prox = nodo;
    }
    
    lista->ult = nodo;
    lista->largo++;
    return true;
}

void *lista_borrar_primero(lista_t *lista) {
    if(!lista || lista_esta_vacia(lista)) return NULL;
    
    nodo_t *nodo_primero = lista->prim;
    void *dato = nodo_primero->dato;
    
    if (lista_largo(lista) == 1) lista->ult = NULL;
    
    lista->prim = nodo_primero->prox;
    lista->largo--;
    nodo_destruir(nodo_primero);
    return dato;
}

void *lista_ver_primero(const lista_t *lista) {
    if(!lista || lista_esta_vacia(lista)) return NULL;
    void *dato = lista->prim->dato;
    return dato;
}

void *lista_ver_ultimo(const lista_t* lista) {
    if(!lista || lista_esta_vacia(lista)) return NULL;
    void *dato = lista->ult->dato;
    return dato;
}

size_t lista_largo(const lista_t *lista) {
    return lista->largo;
}


void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
    nodo_t *actual = lista->prim;
    bool corte = true;
    while (actual != NULL && corte) {
        corte = visitar(actual->dato, extra);
        actual = actual->prox;
    }
}


/*----------------------------------------- */

/* Defincion del struct lista_iter */
struct lista_iter {
    lista_t *lista;
    nodo_t *actual;
    nodo_t *anterior;
};

/* Primitivas del iterador */

lista_iter_t *lista_iter_crear(lista_t *lista) {
    lista_iter_t *iter = malloc(sizeof(lista_iter_t));
    
    if (iter == NULL) return NULL;

    iter->lista = lista;
    iter->actual = lista->prim;
    iter->anterior = NULL;
    
    return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
    if(lista_iter_al_final(iter)) return false;
    
    iter->anterior = iter->actual;
    iter->actual = iter->actual->prox;
    return true;
}

void *lista_iter_ver_actual(const lista_iter_t *iter) {
    if (lista_iter_al_final(iter)) return NULL;
    return iter->actual->dato;
}

bool lista_iter_al_final(const lista_iter_t *iter) {
    return iter->actual == NULL;
}

void lista_iter_destruir(lista_iter_t *iter) {
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
    nodo_t *nodo = nodo_crear(dato);
    if (nodo == NULL) return false;

    if (lista_iter_al_final(iter)) iter->lista->ult = nodo;
    
    if (iter->anterior) iter->anterior->prox = nodo;
        
    else {
        iter->lista->prim = nodo;
    }
    
    nodo->prox = iter->actual;
    iter->actual = nodo;
    iter->lista->largo++;
    return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
    if (lista_iter_al_final(iter)) return NULL;
    
    nodo_t *nodo = iter->actual;
    void *dato = nodo->dato;
    if (nodo == NULL) return NULL;
    
    if (nodo == iter->lista->prim) {
        iter->lista->prim = nodo->prox;
    }

    else if (nodo->prox == NULL) {
        iter->lista->ult = iter->anterior;
        iter->anterior->prox = NULL;
    }
    else {
        iter->anterior->prox = nodo->prox;
    }

    iter->lista->largo--;
    iter->actual = nodo->prox;
    nodo_destruir(nodo);
    return dato;
}