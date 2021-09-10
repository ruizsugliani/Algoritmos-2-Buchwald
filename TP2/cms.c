#include "hfns.h"
#include "cms.h"
#define  CANT_HASHES 5
#define  LEN_ARRAY 97

/* ******************************************************************
 *                        COUNT MIN SKETCH
 * *****************************************************************/

struct cms {
    hashes_t* funciones_hash;
    size_t **tablas_hash;
};

cms_t *cms_crear(){
    cms_t *cms = malloc(sizeof(cms_t));
    if (!cms) return NULL;
 
    hashes_t *hash_funciones = malloc(sizeof(hashes_t) * CANT_HASHES);
    if (!hash_funciones){
        free(cms);
        return NULL;
    }

    size_t **hash_tablas = calloc(CANT_HASHES, sizeof(size_t*));
    if (!hash_tablas){
        free(cms);
        free(hash_funciones);
        return NULL;
    }
    cms->tablas_hash = hash_tablas;

    for (size_t j = 0; j < CANT_HASHES; j++){
        size_t *hash_tabla = calloc(LEN_ARRAY, sizeof(size_t));
        if (!hash_tabla){
            free(cms);
            free(hash_funciones);
            return NULL;
        }
        cms->tablas_hash[j] = hash_tabla;
    }

    cms->funciones_hash = hash_funciones;
    hashes_t funciones[] = {hash_1, hash_2, hash_3, hash_4, hash_5};
    for (size_t i = 0; i < CANT_HASHES; i++){
        cms->funciones_hash[i] = funciones[i];
    }
    return cms;
}

void cms_guardar(cms_t *cms, const char *clave){
    size_t indice;
    for (size_t i = 0; i < CANT_HASHES; i++){
        indice = cms->funciones_hash[i](clave) % LEN_ARRAY;
        cms->tablas_hash[i][indice]++;
    }
}

size_t cms_obtener(cms_t *cms, const char *clave){
    size_t indice_inicial = cms->funciones_hash[0](clave) % LEN_ARRAY;
    size_t min = cms->tablas_hash[0][indice_inicial];

    for (size_t i = 1; i < CANT_HASHES; i++){
        size_t indice = cms->funciones_hash[i](clave) % LEN_ARRAY;
        if (cms->tablas_hash[i][indice] < min){
                min = cms->tablas_hash[i][indice];
        }
    }
    return min;
}

void cms_destruir(cms_t *cms) {
    free(cms->funciones_hash);
    for (size_t i = 0; i < CANT_HASHES; i++){
        free(cms->tablas_hash[i]);
    }
    free(cms->tablas_hash);
    free(cms);
}