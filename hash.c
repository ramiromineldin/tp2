#include "hash.h"
#include<stdio.h>
#include <stdlib.h>
#include<string.h>
#define CAPACIDAD_INI 20
#define FACTOR_REDIM 0.6

typedef enum estado {
    VACIO, OCUPADO, BORRADO
} estado_t;

typedef struct campo{
  char* clave;
  void* valor;
  estado_t estado;
} campo_t;

struct hash{
  size_t capacidad;
  size_t cantidad;
  size_t cantidad_borrados;
  hash_destruir_dato_t funcion_destruccion;
  campo_t* tabla;
};
struct hash_iter{
  const hash_t* hash;
  campo_t *campo_act;
  size_t posicion;
};
// video donde aparece la funcion: https://youtu.be/RFTVOzKJGBQ 

size_t fnv_hashing(const char* clave) {
	size_t h = 1469598103934665603;
	size_t n = strlen(clave);
	for(size_t i = 0; i < n; i++) {
		h *= 1099511628211;
		h ^= clave[i];
	}
	return h;
}
/* *****************************************************************
 *                    FUNCIONES AUXILIARES                         *
 * *****************************************************************/

campo_t campo_crear(void* dato, const char *clave, estado_t estado) {
	campo_t campo; 
	campo.valor = dato;
	if (clave !=  NULL) {
		campo.clave = strdup(clave);
	}
	campo.estado = estado;
	return campo;
}
void inicializar_tabla(campo_t* tabla, size_t capacidad) {
    for(size_t i = 0; i < capacidad; i++) {
            tabla[i] = campo_crear(NULL, NULL, VACIO);
    }
}

float factor_redimension(size_t cantidad, size_t capacidad) {
    float factor = (float) cantidad / (float) capacidad;
    return factor;
}
void redimensionar_hash(hash_t* hash, size_t capacidad) {
    campo_t *tabla_nueva = malloc(capacidad * sizeof(campo_t)), *tabla_aux;
    if(!tabla_nueva) return;
   	size_t i = 0, j = 0;
    tabla_aux = hash->tabla;
    inicializar_tabla(tabla_nueva, capacidad);
    hash->tabla = tabla_nueva;
    hash->capacidad = capacidad; 
    while(i != hash->cantidad) { 
        if (tabla_aux[j].estado == OCUPADO) {
            hash_guardar(hash, tabla_aux[j].clave, tabla_aux[j].valor);
            hash->cantidad--;
            free(tabla_aux[j].clave);
            i++;
        }
        j++;
    } 
    free(tabla_aux);
}

bool calcular_index (const hash_t* hash, const char* clave, size_t *hash_index) {
	while(hash->tabla[*hash_index].estado != VACIO) {
		if(hash->tabla[*hash_index].estado ==  OCUPADO && strcmp(hash->tabla[*(hash_index)].clave, clave) == 0) {
			return true;
		}
		*(hash_index) += 1;
		*hash_index %= hash->capacidad;
	}
	return false;
}


/* *****************************************************************
 *                    PRIMITIVAS DEL HASH                          *
 * *****************************************************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato) {
	hash_t* hash = malloc(sizeof(hash_t));
		if(!hash)
			return NULL;
		if((hash->tabla = malloc(CAPACIDAD_INI * sizeof(campo_t))) == NULL) {
			free(hash);
			return NULL;
		}
		hash->capacidad = CAPACIDAD_INI;
		hash->cantidad = 0;
		hash->cantidad_borrados = 0;
		hash->funcion_destruccion = destruir_dato;
		inicializar_tabla(hash->tabla, hash->capacidad);

	return hash;
}

bool hash_guardar(hash_t *hash, const char *clave, void *dato) {
	if (factor_redimension(hash->cantidad + hash->cantidad_borrados, hash->capacidad) > FACTOR_REDIM) {
    	redimensionar_hash(hash, 2 * hash->capacidad);
    }
    size_t hash_index = fnv_hashing(clave) % hash->capacidad; 
    if (calcular_index(hash, clave, &hash_index)) { 
    	if (hash->funcion_destruccion != NULL) { 
			hash->funcion_destruccion(hash->tabla[hash_index].valor);
		}
		free(hash->tabla[hash_index].clave); 
	}
    else {
		hash->cantidad++;
	}
    hash->tabla[hash_index] = campo_crear(dato, clave, OCUPADO); 
    return true;
}

void *hash_borrar(hash_t *hash, const char *clave) {
	if (factor_redimension(hash->cantidad + hash->cantidad_borrados, hash->capacidad) > FACTOR_REDIM) {
    	redimensionar_hash(hash, hash->capacidad / 2);
    }
	size_t hash_index = fnv_hashing(clave) % hash->capacidad; 
	while(hash->tabla[hash_index].estado != VACIO) {
		if((hash->tabla[hash_index].estado == OCUPADO) && (strcmp(hash->tabla[hash_index].clave, clave) == 0)) {
			hash->tabla[hash_index].estado = BORRADO;
			hash->cantidad--;
			hash->cantidad_borrados++;
			free(hash->tabla[hash_index].clave); 
			return hash->tabla[hash_index].valor;
		}
		hash_index++;
		hash_index %= hash->capacidad;
	}
    return NULL; 
}

void *hash_obtener(const hash_t *hash, const char *clave) {
	size_t hash_index = fnv_hashing(clave) % hash->capacidad;
	if (calcular_index(hash, clave, &hash_index)) {
		return hash->tabla[hash_index].valor;
	}
	return NULL; 
}

bool hash_pertenece(const hash_t *hash, const char *clave) {
	size_t hash_index = fnv_hashing(clave) % hash->capacidad;
	if (calcular_index(hash, clave, &hash_index)) {
		return true;
	}
	return false;
}

size_t hash_cantidad(const hash_t *hash) {
	return hash->cantidad;
}

void hash_destruir(hash_t *hash) {
    size_t i = 0;
    while (i < hash->capacidad) {
        if (hash->tabla[i].estado == OCUPADO) {
            if (hash->funcion_destruccion) hash->funcion_destruccion(hash->tabla[i].valor);
            free(hash->tabla[i].clave);
        }
        i++;  
    }
    free(hash->tabla);
    free(hash);
}
/* *****************************************************************
 *                    PRIMITIVAS DEL ITERADOR                      *
 * *****************************************************************/

hash_iter_t *hash_iter_crear(const hash_t *hash) {
    hash_iter_t *iter = malloc(sizeof(hash_iter_t));
    
    if (!iter) return NULL;
    iter->hash = hash;
    iter->posicion = 0;
    
    if (hash->tabla[0].estado != OCUPADO) {
        while(iter->posicion < hash->capacidad && hash->tabla[iter->posicion].estado != OCUPADO) {
            iter->posicion++;
        }
        if (iter->posicion == iter->hash->capacidad) {
            iter->campo_act = NULL;
        }
        else {
            iter->campo_act = &(hash->tabla[iter->posicion]);
        }
    }

    else {
        iter->campo_act = &(hash->tabla[0]);
    }
    return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter) {
    if (hash_iter_al_final(iter)) return false;
    iter->posicion++;
    while (iter->posicion < iter->hash->capacidad && iter->hash->tabla[iter->posicion].estado != OCUPADO) {
        iter->posicion++;
    }
    if (iter->posicion == (iter->hash->capacidad)) {
        iter->campo_act = NULL;
        return true;
    }
    
    iter->campo_act = &(iter->hash->tabla[iter->posicion]);
    return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter) {
    if (hash_iter_al_final(iter)) return NULL;
    return iter->campo_act->clave;
}

bool hash_iter_al_final(const hash_iter_t *iter) {
    return iter->campo_act == NULL;
}

void hash_iter_destruir(hash_iter_t *iter) {
    free(iter);
}
