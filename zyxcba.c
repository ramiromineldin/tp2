#include <stdio.h>
#include <string.h>
#include "funciones_tp2.h"
#include "strutil.h"
#include "mensajes.h"

#define COMANDO_PEDIR_TURNO "PEDIR_TURNO"
#define COMANDO_ATENDER "ATENDER_SIGUIENTE"
#define COMANDO_INFORME "INFORME"


bool validar_cantidad_parametros (char** parametros, size_t cant) {
	size_t contados = 0;
	while (parametros[contados]) {
		contados++;
	}
	return contados = cant;
}

void procesar_comando(const char* comando, const char** parametros, abb_t *doctores, hash_t *pacientes, hash_t *especialidades) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		if (!validar_cantidad_parametros(parametros, 3)) printf(ENOENT_PARAMS, comando);
		else pedir_turno(pacientes, especialidades, parametros);

	} 
	else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		if (!validar_cantidad_parametros(parametros, 1)) printf(ENOENT_PARAMS, comando);
		else atender_siguiente_paciente(doctores, pacientes, especialidades, parametros);

	} 
	else if (strcmp(comando, COMANDO_INFORME) == 0) {
		if (!validar_cantidad_parametros(parametros, 2)) printf(ENOENT_PARAMS, comando);


	} 
	else {
		printf(ENOENT_CMD, comando);
	}
}

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_entrada(abb_t *doctores, hash_t *pacientes, hash_t *especialidades) {
	char* linea = NULL;
	size_t c = 0;
	while (getline(&linea, &c, stdin) > 0) {
		eliminar_fin_linea(linea);
		char** campos = split(linea, ':');
		if (campos[1] == NULL) {
			printf(ENOENT_FORMATO, linea);
			free_strv(campos);
			continue;	
		}
		char** parametros = split(campos[1], ',');
		procesar_comando(campos[0], parametros);
		free_strv(parametros);
		free_strv(campos);
	}
	free(linea);
}



int main(int argc, char** argv) {
	if (argc != 3) {
		printf(ENOENT_CANT_PARAMS);
		return 1;
	}
	hash_t *especialidades = hash_crear(destruir_especialidad);
	if (!especialidades) return 1;
	abb_t *doctores = guardar_doctores(argv[0], especialidades, cmp_anio_ingreso);
	if (!doctores) return 1
	hash_t *pacientes = guardar_pacientes(argv[1]);
	if (!pacientes) return 1;

	procesar_entrada(docores, pacientes, especialidades);

	hash_destruir(especialidades);
	abb_destruir(doctores);
	hash_destruir(pacientes);
	
	return 0;
}
