#include <stdio.h>
#include "mensajes.h"
#include <stdlib.h>


/* Recibe un hash con los pacientes, un hash con las especialidades y los parametros del comando y pide un turno.
   En caso de error imprime un mensaje */
void pedir_turno(hash_t *pacientes, hash_t *especialidades, const char **parametros);

/* Recibe un abb con los doctores, un hash con los pacientes, un hash con las especialidades y los parametrso del comando 
   y atiende al siguiente paciente.
   En caso de error imprime un mensaje */
void atender_siguiente_paciente(abb_t *doctores, hash_t *pacientes, hash_t *especialidades, const char **parametros);

bool imprimir_informes(const char *nombre_doctor, void *doctor, void *extra, size_t *cant);
void guardar_informes(abb_t *doctores, const char **parametros);