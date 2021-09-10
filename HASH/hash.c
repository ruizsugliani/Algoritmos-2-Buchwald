#define _POSIX_C_SOURCE 200809L

#include "hash.h"
#include "lista.h"
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#define CAPACIDAD_INCIAL 11
#define CAPACIDAD_MINIMA 5
#define FACTOR_DE_CARGA 2

/* ******************************************************************
 *                        FUNCION DE HASH
 * *****************************************************************/
//FUENTE DE LA FUNCION: https://en.wikipedia.org/wiki/Jenkins_hash_function
size_t funcion_hash(const char *key, size_t length) {
  size_t i = 0;
  size_t hash = 0;
  while (i != length) {
    hash += key[i++];
    hash += hash << 10;
    hash ^= hash >> 6;
  }
  hash += hash << 3;
  hash ^= hash >> 11;
  hash += hash << 15;
  return hash;
}

struct hash{
    size_t cantidad;
    size_t capacidad;
    lista_t **tabla;
    hash_destruir_dato_t destruir_dato;
};

typedef struct hash_campo{
    char *clave;
    void *valor;
}hash_campo_t;

struct hash_iter{
    lista_iter_t *lista_iter;
    lista_t *lista;
    const hash_t* hash;
    size_t indice;
};

/* ******************************************************************
 *                        AUXILIARES
 * *****************************************************************/
/*
Encuentra el elemento en el hash en el caso de que estuviera y lo devuelve, si se pasa true por el parametro 'borrar'
el elemento encontrado se borrara. Si no se encuentra el elemento se duelve NULL
Pre: el hash fue creado
Post : se devolvio el elemento y se borro en caso de ser solicitado.
*/
hash_campo_t *encontrar_elemento(const hash_t *hash, const char *clave, bool borrar){
    size_t indice = funcion_hash(clave, strlen(clave)) % hash->capacidad;
    if (!hash->tabla[indice]){
        return NULL;
    }

    lista_iter_t *iter = lista_iter_crear(hash->tabla[indice]);

    while (!lista_iter_al_final(iter)){
        hash_campo_t *campo_actual = lista_iter_ver_actual(iter);
    
        if (strcmp(campo_actual->clave, clave) == 0){
            if (borrar){
                lista_iter_borrar(iter);
                if (lista_esta_vacia(hash->tabla[indice])){
                    lista_destruir(hash->tabla[indice], NULL);
                    hash->tabla[indice] = NULL;
                }
            }
            lista_iter_destruir(iter);
            return campo_actual;
        }
        lista_iter_avanzar(iter);
    }
    lista_iter_destruir(iter);

    return NULL;    
}

/*
Encuentra una posicion valida para el iterador del hash.
True si hay una posicion valida, false en caso contrario
Pre : el iter fue creado.
Post : el indice del iter se encuentra en la siguiente posicion disponible.
*/
bool encontrar_posicion_valida(hash_iter_t *iter){
    for (; iter->indice < iter->hash->capacidad; iter->indice++){
        if (iter->hash->tabla[iter->indice] && !lista_esta_vacia(iter->hash->tabla[iter->indice])){
            return true;
        }
    }
    iter->indice = iter->hash->capacidad;
    return false;
}

/*
Se destruye el interior de la tabla de hash, 'dato_destruir' es en caso de que se quiera destruir el dato
y 'guardar' es en caso de redimension para guardar el campo en la nueva tabla.
Pre: el hash fue creado.
Post: se destruye el interior de la tabla y se aplica lo solicitado(dato_destruir / guardar).
*/
void destruir_interior(hash_t *hash, lista_t **tabla, size_t capacidad, bool dato_destruir, bool guardar){
    for (int i = 0; i < capacidad; i++){
        if (tabla[i]){
            while (!lista_esta_vacia(tabla[i])){
                hash_campo_t *campo_aux = lista_borrar_primero(tabla[i]);
               
                if (dato_destruir && hash->destruir_dato){
                    hash->destruir_dato(campo_aux->valor);
                }
                else if (guardar){
                    hash_guardar(hash, campo_aux->clave, campo_aux->valor);
                }
               
                free(campo_aux->clave);
                free(campo_aux);
            }
            lista_destruir(tabla[i],NULL);
        }
    }
}

/*
Redimensiona la tabla del hash en caso de que el factor de carga llegue al tope, es decir el doble del tamaño de la tabla.
Pre: la tabla tiene un factor de carga del doble de su tamaño.
Post: se redimensiono la tabla voliendo a guardar los elementos en la nueva tabla.
*/
bool hash_redimensionar(hash_t *hash, size_t tam_nuevo){
    if (tam_nuevo < CAPACIDAD_MINIMA){
        tam_nuevo = CAPACIDAD_MINIMA;
    }

    lista_t **tabla_aux = hash->tabla;
    size_t capacidad_aux = hash->capacidad;

    lista_t **aux = calloc(tam_nuevo, sizeof(lista_t*));
    if (!aux){
        return false;
    }
    
    hash->tabla = aux;
    hash->capacidad = tam_nuevo;
    hash->cantidad = 0;

    destruir_interior(hash, tabla_aux, capacidad_aux, false, true);

    free(tabla_aux);
    return true;
}

/* ******************************************************************
 *                        PRIMITIVAS
 * *****************************************************************/

hash_t *hash_crear(hash_destruir_dato_t destruir_dato){
    hash_t *hash = malloc(sizeof(hash_t));
    if (!hash){
        return NULL;
    }

    hash->tabla = calloc(CAPACIDAD_INCIAL, sizeof(void *));
    if (!hash->tabla){
        free(hash);
        return NULL;
    }
    for (int i = 0; i < CAPACIDAD_INCIAL; i++){
        hash->tabla[i] = NULL;
    }

    hash->cantidad = 0;
    hash->capacidad = CAPACIDAD_INCIAL;
    hash->destruir_dato = destruir_dato;
    return hash;
}

size_t hash_cantidad(const hash_t *hash){
    return hash->cantidad;
}

void hash_destruir(hash_t *hash){
    destruir_interior(hash, hash->tabla, hash->capacidad, true, false);
    free(hash->tabla);
    free(hash);
}


bool hash_guardar(hash_t *hash, const char *clave, void *dato){
    //Caso de redimensión.
    if (hash->cantidad == (hash->capacidad * FACTOR_DE_CARGA)){
        if (!hash_redimensionar(hash, hash->capacidad * FACTOR_DE_CARGA)){
            return false;
        }   
    }
    size_t indice = funcion_hash(clave, strlen(clave)) % hash->capacidad;

    hash_campo_t *reemplazar = encontrar_elemento(hash, clave, false);
    if (reemplazar){
        if (hash->destruir_dato){
            hash->destruir_dato(reemplazar->valor);
        }
        reemplazar->valor = dato;
        return true;
    }

    char *duplicado = strdup(clave);
    if (!duplicado){
        return false;
    }

    hash_campo_t *campo = malloc(sizeof(hash_campo_t));
    if (!campo){
        free(duplicado);
        return false;
    }
    campo->clave = duplicado;
    campo->valor = dato;

    if (!hash->tabla[indice]){
        lista_t *lista = lista_crear();
        hash->tabla[indice] = lista;
        lista_insertar_ultimo(lista, campo);
    }
    else{
        lista_insertar_ultimo(hash->tabla[indice], campo);
    }

    hash->cantidad++;
    return true;
}

bool hash_pertenece(const hash_t *hash, const char *clave){
    hash_campo_t *campo = encontrar_elemento(hash, clave, false);
    if (campo){
        return true;
    }
    return false;
}

void *hash_obtener(const hash_t *hash, const char *clave){
    hash_campo_t *campo = encontrar_elemento(hash, clave, false);
    if (campo){
        void *valor = campo->valor;
        return valor;
    }
    return NULL;
}

void *hash_borrar(hash_t *hash, const char *clave){
    if (hash->cantidad > CAPACIDAD_MINIMA && hash->cantidad * FACTOR_DE_CARGA * FACTOR_DE_CARGA <= hash->capacidad){
        if (!hash_redimensionar(hash, hash->capacidad / FACTOR_DE_CARGA)){
            return false;
        }
    }
    
    hash_campo_t *campo = encontrar_elemento(hash, clave, true);
    if (campo){
        void *valor_viejo = campo->valor;
        free(campo->clave);
        free(campo);
        hash->cantidad--;
        return valor_viejo;
    }

    return NULL;
}

/* ******************************************************************
 *                        ITERADOR
 * *****************************************************************/

hash_iter_t *hash_iter_crear(const hash_t *hash){
    hash_iter_t *iter = malloc(sizeof(hash_iter_t));
    if (!iter){
        return NULL;
    }

    iter->indice = 0;
    iter->hash = hash;
    iter->lista = NULL;
    iter->lista_iter = NULL;

    if (encontrar_posicion_valida(iter)){
        iter->lista = iter->hash->tabla[iter->indice];
        iter->lista_iter = lista_iter_crear(iter->lista);
    }
    return iter;
}

bool hash_iter_avanzar(hash_iter_t *iter){
    if (hash_iter_al_final(iter)){
        return false;
    }
  
    lista_iter_avanzar(iter->lista_iter);

    if (lista_iter_al_final(iter->lista_iter)){
        lista_iter_destruir(iter->lista_iter);
        iter->indice++;
        if (!encontrar_posicion_valida(iter)){
            iter->lista_iter = NULL;
            return false;
        }
        iter->lista = iter->hash->tabla[iter->indice];
        iter->lista_iter = lista_iter_crear(iter->lista);
    }

    return true;
}

const char *hash_iter_ver_actual(const hash_iter_t *iter){
    if (hash_iter_al_final(iter)){
        return NULL;
    }

    hash_campo_t *campo_actual = lista_iter_ver_actual(iter->lista_iter);
    char *clave = campo_actual->clave; 
    return clave;
}

bool hash_iter_al_final(const hash_iter_t *iter){
    return iter->indice == iter->hash->capacidad;
}

void hash_iter_destruir(hash_iter_t *iter){
    if (iter->lista_iter){
        lista_iter_destruir(iter->lista_iter);
    }
    free(iter);
}


//EJERCICIOS
//Implementar, para un Hash Abierto, una primitiva que dado un Hash, un número k y una función void aplicar(void*
//dato), aplique la función pasada por parámetro a los primeros k datos en el Hash. En caso de que el Hash tenga
//menos de k elementos, aplicar la función aplicar a todos los datos del Hash. Indicar y justificar la complejidad de la
//primitiva implementada. La firma de la primitiva deberá ser: bool aplicar_selectivo(const hash_t* hash, int k,
//void (*aplicar)(void* dato))
/*
int aplicar(void* dato) {
    return *(int*) dato * 2;
}

bool aplicar_selectivo(const hash_t* hash, int k, int (*aplicar)(void* dato)) {
    if (k > hash->capacidad) return false;
    int contador = 0;
    hash_iter_t* iter = hash_iter_crear(hash);
    if (!iter) return NULL;

    for (int i = 0; contador < k; i++) {
        if (hash->tabla[i]) {
            lista_iter_t* iter = lista_iter_crear(hash->tabla[i]);
            while (!lista_iter_al_final(iter)) {
                hash_campo_t* campo_actual = lista_iter_ver_actual(iter);
                campo_actual->valor = aplicar(campo_actual->valor);
                lista_iter_avanzar(iter);
                contador++;
            }
        }
    }
    return true;
}

int main() {
    const hash_t* hash = hash_crear(NULL);
    int numeros[] = {1, 2, 3, 4, 5};
    hash_guardar(hash, "a", &numeros[0]);
    hash_guardar(hash, "b", &numeros[1]);
    hash_guardar(hash, "c", &numeros[2]);
    hash_guardar(hash, "d", &numeros[3]);
    hash_guardar(hash, "e", &numeros[4]);

    hash_iter_t* iterador1 = hash_iter_crear(hash);
    while(!hash_iter_al_final(iterador1)) {
        const char* clave = hash_iter_ver_actual(iterador1);
        printf("Clave : %s\n", clave);
        void* valor = hash_obtener(hash, clave);
        printf("Valor : %d\n", *(int*) valor);
        hash_iter_avanzar(iterador1);
    }
    //hash_iter_t* iterador2 = hash_iter_crear(hash);
    return 0;
}

hash_t* mejores_valores(hash_t* h1, hash_t* h2) {
    hash_t* res = hash_crear(NULL);
    hash_iter_t* iter1 = hash_iter_crear(h1);
    hash_iter_t* iter2 = hash_iter_crear(h2);
    //Comienzo con el primer hash
    while (!hash_iter_al_final(iter1)) {
        char* producto = hash_iter_ver_actual(iter1);
        size_t* precio = hash_obtener(h1, producto);
        if (!hash_pertenece(res, producto)) {
            hash_guardar(res, producto, precio);
        }
        hash_iter_avanzar(iter1);
    }
    hash_iter_destruir(iter1);
    //Lo mismo con el segundo comprobando mejores valores
    while(!hash_iter_al_final(iter2)) {
        char* producto = hash_iter_ver_actual(iter2);
        size_t* precio = hash_obtener(h2, producto);
        if (!hash_pertenece(res, producto)) {
            hash_guardar(res, producto, precio);
        } else {
            size_t precio_ant = *(size_t*) hash_obtener(res, producto);
            if (precio_ant > *precio) {
                hash_guardar(res, producto, precio);
            }
        }
        hash_iter_avanzar(iter2);
    }
    hash_iter_destruir(iter2);
    return res;
}
*/