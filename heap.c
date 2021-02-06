#include "heap.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define CAPACIDAD_INI 20
#define FACTOR_REDIM 2
#define FACTOR_REDIM_AUX 4

struct heap {
	void** datos; 
	size_t tam;
	size_t cant;
	cmp_func_t comparar; 
}; 

/* *****************************************************************
 *                     FUNCIONES AUXILIARES                        *
 * *****************************************************************/
static bool heap_redimensionar(heap_t *heap, size_t tamanio_nuevo) {
    void **datos_nuevo = realloc(heap->datos, (tamanio_nuevo * sizeof(void*)));
    if (!datos_nuevo) return false; 

    heap->tam = tamanio_nuevo; 
    heap->datos = datos_nuevo;
    return true;
}
void buscar_maximo (void* arreglo[], size_t izq, size_t der, size_t actual, size_t *mayor, size_t tam, cmp_func_t comparar) {
	if ((izq < tam) &&  (comparar(arreglo[izq],arreglo[actual]) >= 0)) {
		*mayor = izq;
		if (der < tam && (comparar(arreglo[izq], arreglo[der]) < 0)) {
			*mayor = der;
		}
	}
	else if ((der < tam) && (comparar(arreglo[der], arreglo[actual]) > 0)) {
		*mayor = der;
		if(comparar(arreglo[izq], arreglo[der]) > 0) {
			*mayor = izq;
		}
	}

}
void posicion_hijos(size_t *der, size_t *izq, size_t actual) {
	*izq = 2 * actual + 1; 
	*der = 2 * actual + 2;
}
void swap(void** x, void** y) {
    void *aux = *x;
    *x = *y;
    *y = aux;
}
void downheap (void* arreglo[],size_t tam, size_t actual, cmp_func_t comparar) {
	if (actual >= tam) {
		return;
	}
	size_t mayor = actual, der, izq; 
	posicion_hijos(&der, &izq, mayor);
	buscar_maximo(arreglo, izq, der, actual, &mayor, tam, comparar);
	if (mayor != actual) {
		swap(&arreglo[actual], &arreglo[mayor]);
		downheap(arreglo, tam, mayor, comparar);
	}
}

void heapify(void* arreglo[], size_t tam, cmp_func_t cmp) {
	for (size_t i = tam/2; i > 0; i--) {
		downheap(arreglo,tam, i, cmp);
	}
	downheap(arreglo,tam, 0, cmp);
}

size_t calcular_posicion_padre(size_t posicion_hijo) {
    return (posicion_hijo - 1) / 2;
}

void upheap(void** datos, size_t posicion_hijo, cmp_func_t comparar) {
    if (posicion_hijo == 0) return;
    size_t posicion_padre = calcular_posicion_padre(posicion_hijo);

    if (comparar(datos[posicion_padre], datos[posicion_hijo]) < 0) {
        swap(&datos[posicion_padre], &datos[posicion_hijo]);
        upheap(datos, posicion_padre, comparar);
    }
    return;
}
void heap_sort(void *elementos[], size_t cant, cmp_func_t cmp) {
    heapify(elementos,cant, cmp);
    for (size_t i = cant-1; i > 0; i--) {
        swap(&elementos[i], &elementos[0]);
		downheap(elementos, i, 0, cmp);
   }
}
/* *****************************************************************
 *                     PRIMITIVAS DEL HEAP                         *
 * *****************************************************************/
heap_t *heap_crear(cmp_func_t cmp) {
    heap_t *heap = malloc(sizeof(heap_t));
    if (!heap) return NULL;
    
    void** datos_nuevos = malloc(CAPACIDAD_INI * sizeof(void*));
    if (!datos_nuevos) {
        free(heap);
        return NULL;
    }
    
    heap->datos = datos_nuevos;
    heap->tam = CAPACIDAD_INI;
    heap->cant = 0;
    heap->comparar = cmp;
    return heap;
}

heap_t *heap_crear_arr(void *arreglo[], size_t n, cmp_func_t cmp) {
	heap_t* heap = malloc(sizeof(heap_t));
	if(!heap) return NULL;
	heap->datos = malloc(n * sizeof(void*));
	if (!heap->datos) {
		free(heap);
		return NULL; 
	}
	heap->cant = n;
	heap->tam = n;
	heap->comparar = cmp;
	for (size_t i = 0; i < n; i++) {
		heap->datos[i] = arreglo[i];
	}
	heapify(heap->datos,n,cmp);
	return heap;
}
bool heap_esta_vacio(const heap_t *heap) {
	return heap->cant == 0;
}
bool heap_encolar(heap_t *heap, void *elem) {
    if (elem == NULL) return false;
    
    if (heap->tam == heap->cant) {
        size_t tamanio_nuevo = heap->tam * FACTOR_REDIM;
        if (!heap_redimensionar(heap, tamanio_nuevo)) return false;
    }

    heap->datos[heap->cant] = elem;
    upheap(heap->datos, heap->cant, heap->comparar);
    heap->cant++;
    return true;
}

void *heap_desencolar(heap_t *heap) {
	if (heap_esta_vacio(heap)) return NULL;
	if ((heap->tam / FACTOR_REDIM_AUX) >= CAPACIDAD_INI && heap->tam == (heap->cant * FACTOR_REDIM_AUX)) {
       	size_t tamanio_nuevo = heap->tam / FACTOR_REDIM;
       	if (!heap_redimensionar(heap, tamanio_nuevo)) return false;
    }

	void* aux = heap->datos[0];
	swap(&heap->datos[0], &heap->datos[(heap->cant)-1]);
	heap->cant--; 
	downheap(heap->datos, heap->cant, 0, heap->comparar);
	return aux;
}

void heap_destruir(heap_t *heap, void (*destruir_elemento)(void *e)) {
    while(!heap_esta_vacio(heap)) {
    	void* dato = heap_desencolar(heap);
        if (destruir_elemento != NULL) {
			destruir_elemento(dato);
		}
	}
	free(heap->datos);
	free(heap);
}
void *heap_ver_max(const heap_t *heap) {
	if (heap_esta_vacio(heap)) return NULL; 
	return heap->datos[0];
}

size_t heap_cantidad(const heap_t *heap) {
	return heap->cant;
}

