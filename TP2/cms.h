#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct cms cms_t;

/* ******************************************************************
 *                 PRIMITIVAS DE COUNT MIN SKETCH
 * *****************************************************************/

//Crea un arreglo con funciones de hash en cada posición y otro arreglo de arreglos 
//(correspondiente a cada función de hash) donde se guardarán las ocurrencias, finalmente lo devuelve.
//Queda a cargo del usuario destruir la estructura. 
cms_t *cms_crear();

//Toma por parámetro la estructura y una clave para sumar una unidad 
//en el arreglo de cada función de hash tras aplicarle dichas funciones a la clave.
void cms_guardar(cms_t *cms, const char *clave);

//Destruye la estructura liberando toda la memoria pedida en cms_crear().
void cms_destruir(cms_t *cms);

size_t cms_obtener(cms_t *cms, const char *clave);