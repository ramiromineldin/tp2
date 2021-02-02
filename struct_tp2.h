#include <stdlib.h>
#include "csv.h"
#include "mensajes.h"

typedef struct paciente paciente_t;
typedef struct doctor doctor_t;
typedef struct especialidad especialidad_t;

/* Crea un paciente
Recibe el nombre y el año de inscripcion del paciente y devuelve un paciente_t con la informacion.
En caso de error devuleve NULL
*/
paciente_t *crear_paciente(char **campos, void *extra);

/* Crea un doctor
Recibe el nombre y la especialidad del doctor y devuelve un doctor_t con la informacion.
En caso de error devuleve NULL
*/
doctor_t *crear_doctor(char **campos, void *extra);

/* Crea una especialidad
Recibe el nombre de la especialidad y una funcion de comparacion y  devuelve una especialidad_t con la informacion.
En caso de error devuleve NULL
*/
especialidad_t *crear_especialidad(cmp_func_t comparar, char *nombre);

/* Recibe la ruta de un archivo csv y devuelve un hash con todos los pacientes. 
En caso de error devuelve NULL */
hash_t *guardar_pacientes(char *ruta_archivo);

/* Recibe la ruta de un archivo csv, un hash y una funcion de comparar y devuelve un abb con todos los doctores. 
En caso de error devuelve NULL */
abb_t *guardar_doctores(char *ruta_archivo, hash_t *especialidades, cmp_func_t comparar);


/* Recibe un paciente y lo destruye */
void destruir_paciente(void *paciente);

/* Recibe una especialidad y la destruye */
void destruir_especialidad(void *especialidad);

/* Recibe un doctor y lo destruye */
void destruir_doctor(void *doctor); 
