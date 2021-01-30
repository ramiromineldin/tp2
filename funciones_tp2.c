#include <stdlib.h>
#include "csv.h"
#include "mensajes.h"

typedef struct paciente {
    char *nombre;
    int *anio;
} paciente_t;

typedef struct doctor {
    char *nombre;
    char *especialidad;
    int *atendidos;
} doctor_t;

typedef struct especialidad {
    char *nombre;
    cola_t *urgentes;
    heap_t *regulares;
} especialidad_t;


paciente_t *crear_paciente(char **campos, void *extra) {
    paciente_t *paciente = malloc(sizeof(paciente_t));
    if (!paciente) return NULL;
    paciente->nombre = strdup(campos[0]);
    paciente->anio = campos[1];
    return paciente;
}

doctor_t *crear_doctor(char **campos, void *extra) {
    doctor_t *doctor = malloc(sizeof(doctor_t));
    if (!doctor) return NULL;
    doctor->nombre = strdup(campos[0]);
    doctor->especialidad = strdup(campos[1]);
    doctor->atendidos = 0;
    return doctor;
}

especialidad_t *crear_especialidad(cmp_func_t comparar, char *nombre, hash_t *especialidades) {
    if (hash_pertenece(especialidades, nombre) return NULL;
        
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
    return especialidad
}

hash_t *guardar_pacientes(char *ruta_archivo) {
    hash_t *pacientes = hash_crear(destruir_paciente);
    if (!pacientes) return NULL;
    
    lista_t *lista_pacientes = csv_crear_estructura(ruta_archivo, crear_paciente, NULL);
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

abb_t *guardar_doctores(char *ruta_archivo, hash_t *especialidades) {
    
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
        crear_especialidad(strcmp, doctor->especialidad, especialidades);
        hash_guardar(especialidades, doctor->especialidad);
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
