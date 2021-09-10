#include <stdbool.h>
#include <stdio.h>
//1
/*
Por DyC determina si un arreglo se encuentra ordenado. Indicar y justificar
el orden del algoritmo, utilizando el Teorema Maestro.
*/
bool _esta_ordenado(int arr[], size_t inicio, size_t fin) {
    if (inicio == fin) {
        return arr[inicio] < arr[inicio + 1];
    }
    size_t medio = (inicio + fin) / 2;
    bool lado_izq = _esta_ordenado(arr, 0, medio);
    bool lado_der = _esta_ordenado(arr, medio + 1, fin);
    return lado_izq && lado_der;
}

//4
struct multiconj {
    hash_t* hash;
};

multiconj_t* multiconj_crear() {
    multiconj_t* multi_conj = malloc(sizeof(multiconj_t));
    if (!multi_conj) return NULL;
    
    multi_conj->hash = hash_crear(NULL);
    if (!multi_conj->hash) {
        free(multi_conj);
        return NULL;
    }

    return multi_conj;
}

bool multiconj_guardar(multiconj_t* multiconj, const char* elem) {
    size_t dato = hash_obtener(multiconj->hash, elem);
    size_t a_guardar = dato + 1;
    return hash_guardar(multiconj->hash, elem, dato + 1);
}

bool multiconj_pertenece(const multiconj_t* multiconj, const char* elem) {
    return hash_pertenece(multiconj->hash, elem);
}

bool multiconj_borrar(multiconj_t* multiconj, char* elem) {
    if (!hash_pertenece(multiconj->hash, elem)) return false;
    size_t dato = hash_obtener(multiconj->hash, elem);
    hash_guardar(multiconj->hash, elem, dato - 1);

    size_t dato_n = hash_obtener(multiconj->hash, elem);
    if (dato_n == 0) {
        hash_borrar(multiconj->hash, elem);
    }
    return true;
}

void multiconj_destruir(multiconj_t* multiconj) {
    hash_destruir(multiconj->hash);
    free(multiconj);
}
