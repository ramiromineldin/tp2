#include "funciones_tp2.h"

#define REGULAR "REGULAR"
#define URGENTE "URGENTE"


void pedir_turno(hash_t *pacientes, hash_t *especialidades, const char **parametros) {
    if (!hash_pertenece(pacientes, parametros[0])) printf(ENOENT_PACIENTE, parametros[0]);
    
    else if (!hash_pertenece(especialidades, parametros[1])) printf(ENOENT_ESPECIALIDAD, parametros[1]);

    else if (strcmp(parametros[2], REGULAR) != 0 && strcmp(parametros[2], URGENTE) != 0) printf(ENOENT_URGENCIA, parametros[2]);

    else {
        especialidad_t *especialidad = hash_obtener(especialidades, parametros[1]);
        paciente_t *paciente = hash_obtener(pacientes, parametros[0]);
        if (strcmp(parametros[2], REGULAR) == 0) heap_encolar(especialidad->regulares, paciente);
        
        else {
            cola_encolar(especialidad->urgentes, paciente);
        }
        especialidad->en_espera++;
        printf(PACIENTE_ENCOLADO, parametros[0]);
        printf(CANT_PACIENTES_ENCOLADOS, espcialidad->en_espera);
    }
}

void atender_siguiente_paciente(abb_t *doctores, hash_t *pacientes, hash_t *especialidades, const char **parametros) {
    if (!abb_pertenece(doctores, parametros[0])) printf(ENOENT_DOCTOR, parametros[0]);

    doctor_t *doctor = abb_obtener(doctores, parametros[0]);
    especialidad_t *especialidad = hash_obtener(especialidades, doctor->especialidad);
    if (especialidad->en_espera == 0) printf(SIN_PACIENTES);
    paciente_t *paciente
    if (!cola_esta_vacia(especialidad->urgentes)) {
        paciente = cola_desencolar(especialidad->urgentes);
    }
    else {
        paciente = heap_desencolar(especialidad->regulares);
    }
    
    printf(PACIENTE_ATENDIDO, paciente->nombre);
    especialidad->en_espera--;
    printf(CANT_PACIENTES_ENCOLADOS, dcotor->especialidad);    
    doctor->atendidos++;
    hash_borrar(pacientes, paciente->nombre);
    destruir_paciente(paciente);

}

/* visitar */
bool imprimir_informes(const char *nombre_doctor, void *doctor, void *extra, size_t *cant) {
    doctor_t *doctor = doctor;
    if (strcmp(nombre_doctor, extra) != 0) {
        *cant++;
        printf(INFORME_DOCTOR,*cant, nombre_doctor, doctor->especialidad, doctor->atendidos)
        return true
    }
    return false
}

void guardar_informes(abb_t *doctores, const char **parametros) {
    printf(DOCTORES_SISTEMA, abb_cantidad(doctores));
    size_t cant = 0
    abb_in_order(doctores, imprimir_informes, parametros[0], parametros[1], &cant);
}
