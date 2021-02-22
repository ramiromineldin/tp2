 
#include "struct_tp2.h"
#include <stdio.h>
#include "csv.h"
#include <string.h>


struct paciente {
    char *nombre;
    int *anio;
};

struct doctor {
    char *nombre;
    char *especialidad;
    size_t *atendidos;
};

struct especialidad {
    char *nombre;
    cola_t *urgentes;
    heap_t *regulares;
    size_t en_espera;
};


void* crear_paciente(char **campos, void *extra) {
    paciente_t *paciente = malloc(sizeof(paciente_t));
    if (!paciente) return NULL;
    
    char *eptr;
    long anio = strtol(campos[1], &eptr, 10);
    if (anio == 0) printf(ENOENT_ANIO, campos[1]);

    paciente->nombre = strdup(campos[0]);
    if (!paciente->nombre) return NULL;

    paciente->anio = (int*) anio;
    return paciente;
}

void* crear_doctor(char** campos, void *extra) {
    doctor_t *doctor = malloc(sizeof(doctor_t));
    if (!doctor) return NULL;

    doctor->nombre = strdup(campos[0]);
    if (!doctor->nombre) return NULL;

    doctor->especialidad = strdup(campos[1]);
    if (!doctor->especialidad) return NULL;

    doctor->atendidos = 0;
    return doctor;
}

especialidad_t *crear_especialidad(cmp_func_t comparar, char *nombre) {
    especialidad_t *especialidad = malloc(sizeof(especialidad_t));
    if (!especialidad) return NULL;
    
    cola_t *urgentes = cola_crear();
    heap_t *regulares = heap_crear(comparar);

    if (!urgentes) {
        if (regulares) heap_destruir(regulares, NULL);
        return NULL;
    }
    else if (!regulares) {
        if (urgentes) cola_destruir(urgentes, NULL);
        return NULL;
    }
    especialidad->urgentes = urgentes;
    especialidad->regulares = regulares;
    especialidad->nombre = nombre;
    especialidad->en_espera = 0;
    return especialidad;
}

hash_t *guardar_pacientes(char *ruta_archivo) {
    hash_t *pacientes = hash_crear(destruir_paciente);
    if (!pacientes) return NULL;
    
    lista_t* lista_pacientes = csv_crear_estructura(ruta_archivo, crear_paciente, NULL);
    if (!lista_pacientes) {
        printf(ENOENT_ARCHIVO, ruta_archivo);
        return NULL;
    }
    while (!lista_esta_vacia(lista_pacientes)) {
        paciente_t *paciente = lista_borrar_primero(lista_pacientes);
        hash_guardar(pacientes, paciente->nombre, paciente);
    }
    lista_destruir(lista_pacientes, NULL);
    return pacientes;
}

abb_t *guardar_doctores(char *ruta_archivo, hash_t *especialidades, cmp_func_t comparar) {
    
    abb_t *doctores = abb_crear(strcmp, destruir_doctor);
    if (!doctores) return NULL;

    lista_t *lista_doctores = csv_crear_estructura(ruta_archivo, crear_doctor, NULL);
    if (!lista_doctores) {
        printf(ENOENT_ARCHIVO, ruta_archivo);
        return NULL;
    }
   	while (!lista_esta_vacia(lista_doctores)) {
        doctor_t *doctor = lista_borrar_primero(lista_doctores);
        abb_guardar(doctores, doctor->nombre, doctor);
        especialidad_t* especialidad = crear_especialidad(comparar, doctor->especialidad);
        hash_guardar(especialidades, doctor->especialidad, especialidad);
    }
    lista_destruir(lista_doctores, NULL);
    return doctores;
}

void destruir_paciente(void *paciente) {
    paciente_t *paci = paciente;
    free(paci->nombre);
    free(paci);
}

void destruir_especialidad(void *especialidad) {
    especialidad_t *espe = especialidad;
    cola_destruir(espe->urgentes, NULL);
    heap_destruir(espe->regulares, NULL);
    free(especialidad);
}

void destruir_doctor(void *doctor) {
    doctor_t *doc = doctor;
    free(doc->nombre);
    free(doc->especialidad);
    free(doc);
}
