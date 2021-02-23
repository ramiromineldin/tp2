#include "funciones_tp2.h"
#include "struct_tp2.h"
#include <string.h>

#define REGULAR "REGULAR"
#define URGENTE "URGENTE"
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


void pedir_turno(hash_t *pacientes, hash_t *especialidades, char **parametros) {
    if (!hash_pertenece(pacientes, parametros[0])) printf(ENOENT_PACIENTE, parametros[0]);
    
   else if (!hash_pertenece(especialidades, parametros[1])) printf(ENOENT_ESPECIALIDAD, parametros[1]);

    else if (strcmp(parametros[2], REGULAR) != 0 && strcmp(parametros[2], URGENTE) != 0) printf(ENOENT_URGENCIA, parametros[2]);

    else {
        especialidad_t *especialidad = hash_obtener(especialidades, parametros[1]);
        paciente_t *paciente = hash_obtener(pacientes, parametros[0]);

        if (strcmp(parametros[2], REGULAR) == 0) heap_encolar(especialidad->regulares, paciente);
        else cola_encolar(especialidad->urgentes, paciente);

        especialidad->en_espera++;
        printf(PACIENTE_ENCOLADO, paciente->nombre);
        printf(CANT_PACIENTES_ENCOLADOS, especialidad->en_espera, especialidad->nombre);
    }
}

void atender_siguiente_paciente(abb_t *doctores, hash_t *pacientes, hash_t *especialidades, char **parametros) {
    if (!abb_pertenece(doctores, parametros[0])) {
        printf(ENOENT_DOCTOR, parametros[0]);
        return;
    }

    doctor_t *doctor = abb_obtener(doctores, parametros[0]);
    especialidad_t *especialidad = hash_obtener(especialidades, doctor->especialidad);
    if (especialidad->en_espera == 0) printf(SIN_PACIENTES);
    paciente_t *paciente = NULL;
    if (!cola_esta_vacia(especialidad->urgentes)) {
        paciente = cola_desencolar(especialidad->urgentes);
    }
    else if (!heap_esta_vacio(especialidad->regulares)) {
        paciente = heap_desencolar(especialidad->regulares);
    }
    if (paciente) {
        printf(PACIENTE_ATENDIDO, paciente->nombre);
        especialidad->en_espera--;
        printf(CANT_PACIENTES_ENCOLADOS, especialidad->en_espera, doctor->especialidad);    
        doctor->atendidos++;
        hash_borrar(pacientes, paciente->nombre);
        destruir_paciente(paciente);
    }
}

/* visitar */
bool imprimir_informes(const char *nombre_doctor, void *doctor, void *extra, size_t *cant) {
    doctor_t *doctor_aux = doctor;
    if (strcmp(nombre_doctor, extra) != 0) {
        *cant += 1;
        printf(INFORME_DOCTOR,*(size_t*)cant, nombre_doctor, doctor_aux->especialidad,*(size_t*)doctor_aux->atendidos);
        return true;
    }
    return false;
}
void guardar_informes(abb_t *doctores, char **parametros) {
    size_t cant = 0;
    size_t cant_doctores = 0;

    if (abb_cantidad(doctores) == 0) {
        printf(DOCTORES_SISTEMA, cant_doctores);
       return;
    } 

    // BUSCAR INICIO
    abb_iter_t* iter = abb_iter_in_crear(doctores);
    const char* clave_actual = abb_iter_in_ver_actual(iter);
    const char* clave_comienzo = clave_actual;
    const char* clave_final = parametros[1];
    bool comienzo = false;

    while(!abb_iter_in_al_final(iter)) {
        clave_actual = abb_iter_in_ver_actual(iter); 
        if (strcmp(clave_actual, parametros[0]) < 0) {
            clave_comienzo = clave_actual;
            comienzo = true;
        }
        if (comienzo) cant_doctores++;
        if (strcmp(clave_actual, parametros[1]) < 0) {
            clave_final = clave_actual; 
        }
        abb_iter_in_avanzar(iter);
    }
    abb_iter_in_destruir(iter);
    printf(DOCTORES_SISTEMA, cant_doctores);
    if (strcmp(clave_final, clave_comienzo) == 0) return;

    if (strcmp(clave_final, clave_comienzo) >  0) return;
    abb_in_order(doctores, imprimir_informes, clave_comienzo,clave_final, &cant);
}
