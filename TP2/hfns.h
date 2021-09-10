#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

/* ******************************************************************
 *               DECLARACIÓN DEL ARREGLO DE HASHES 
 * *****************************************************************/

typedef size_t (*hashes_t)(const char *clave);

/* ******************************************************************
 *               FUENTES DE HFNS UTILIZADAS PARA CMS
 * *****************************************************************/

//FUENTE: https://stackoverflow.com/questions/20462826/hash-function-for-strings-in-c
size_t hash_1(const char *str);

//FUENTE: https://stackoverflow.com/questions/20462826/hash-function-for-strings-in-c
size_t hash_2(const char *str);

//FUENTE: https://stackoverflow.com/questions/20462826/hash-function-for-strings-in-c
size_t hash_3(const char *str);

//FUENTE DE LA FUNCION: https://en.wikipedia.org/wiki/Jenkins_hash_function
size_t hash_4(const char *str);

//FUENTE: https://twpower.github.io/160-hash-table-implementation-in-cpp-en
size_t hash_5(const char *str);
