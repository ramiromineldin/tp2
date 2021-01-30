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

void procesar_comando(const char* comando, const char** parametros) {
	if (strcmp(comando, COMANDO_PEDIR_TURNO) == 0) {
		if (!validar_cantidad_parametros(parametros, 3)) printf(ENOENT_PARAMS, comando);

	} 
	else if (strcmp(comando, COMANDO_ATENDER) == 0) {
		if (!validar_cantidad_parametros(parametros, 1)) printf(ENOENT_PARAMS, comando);


	} 
	else if (strcmp(comando, COMANDO_INFORME) == 0) {
		if (!validar_cantidad_parametros(parametros, 2)) printf(ENOENT_PARAMS, comando);


	} 
	else {

	}
}

void eliminar_fin_linea(char* linea) {
	size_t len = strlen(linea);
	if (linea[len - 1] == '\n') {
		linea[len - 1] = '\0';
	}
}

void procesar_entrada() {
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

	return 0;
}
