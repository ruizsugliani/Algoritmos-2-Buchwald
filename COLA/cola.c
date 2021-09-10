#include <stdlib.h>
#include <stddef.h>
#include <stdbool.h>
#include <stdio.h>
#include "pila.h"
#include "pila.c"
//Definición de los struct nodo y cola proporcionados por la cátedra.

typedef struct nodo {
    void* dato;
    struct nodo* prox;
} nodo_t;

typedef struct cola {
    nodo_t* prim;
    nodo_t* ult;
} cola_t;

// Definición de nodo_crear proporcionado por la cátedra.
nodo_t* nodo_crear(void* valor) {
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (nodo == NULL) {
        free(nodo);
        return NULL;
    }
    nodo->dato = valor;
    nodo->prox = NULL;
    return nodo;
}

/* *****************************************************************
 *                    PRIMITIVAS DE LA COLA
 * *****************************************************************/

cola_t *cola_crear(void) {
    cola_t *cola = malloc(sizeof(cola_t));
    if (cola == NULL) return NULL;
    cola->prim = NULL;
    cola->ult = NULL;
    return cola;
}

bool cola_esta_vacia(const cola_t *cola) {
    return cola->prim == NULL;
}

void *cola_ver_primero(const cola_t *cola) {
    if (cola_esta_vacia(cola)) return NULL;
    return cola->prim->dato;
}

bool cola_encolar(cola_t *cola, void *valor) {
    nodo_t *nodo = nodo_crear(valor);
    if (nodo == NULL) return false;
    if (cola_esta_vacia(cola)) {
        cola->prim = nodo;
        cola->ult = nodo;
        return true;
    }
    cola->ult->prox = nodo;
    cola->ult = nodo;
    return true;
}

void *cola_desencolar(cola_t *cola) {
    if (cola_esta_vacia(cola)) return NULL;
    void* dato = cola->prim->dato;
    nodo_t* nodo_ant = cola->prim;
    cola->prim = nodo_ant->prox;
    free(nodo_ant);
    return dato;
}

void cola_destruir(cola_t *cola, void (*destruir_dato)(void *)) {
    while (!cola_esta_vacia(cola)) {
        void* dato = cola_desencolar(cola);
        if (destruir_dato) destruir_dato(dato); 
    }
    free(cola);
}
//gcc -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -o pruebas *.c
//valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas

//EJERCICIOS

void** cola_multiprimeros_primitiva(const cola_t* cola, size_t k) {
    void** res = malloc(sizeof(void*)*k);
    if (!res) return NULL;
    //En cada posicion del arreglo pongo un NULL
    for (int i = 0; i < k; i++) {
        res[i] = NULL;
    }
    nodo_t* act = cola->prim;
    for (int i = 0; i < k && act != NULL; i++) {
        res[i] = act->dato;
        act = act->prox;
    }
    return res;
}

void** cola_multiprimeros_funcion(cola_t* cola, size_t k) {
    void** res = malloc(sizeof(void*) * k);
    cola_t* aux = cola_crear();
    if (!aux) return NULL;
    //En cada posicion del arreglo pongo un NULL
    for (int i = 0; i < k; i++) {
        res[i] = NULL;
    }
    //Ahora me encargo de meter en el arreglo los k elementos encolados y en la aux todos los elementos de la cola original
    for (int i = 0; !cola_esta_vacia(cola); i++) {
        void* dato = cola_desencolar(cola);
        if (i < k) {
            res[i] = dato;
            cola_encolar(aux, dato);
        } else {
            cola_encolar(aux, dato);
        }
    }
    while (!cola_esta_vacia(aux)) cola_encolar(cola, cola_desencolar(aux));
    return res;
}

bool wrapper_filtro(void* elemento) {
    if (*(int*) elemento % 2 != 0) return true;
    else return false;
}

void cola_filtrar(cola_t* cola, bool (*filtro)(void*)) {
    cola_t* aux = cola_crear();
    while (!cola_esta_vacia(cola)) {
        void* dato = cola_desencolar(cola);
        if (filtro(dato)) cola_encolar(aux, dato);
    } 

    while (!cola_esta_vacia(aux)) cola_encolar(cola, cola_desencolar(aux));   
}

//EJERCICIOS
/*
Dadas dos colas de enteros positivos (con posibles valores repetidos) cuyos elementos fueron
ingresados de menor a mayor, se pide implementar una función int* merge_colas(cola*
cola_1,cola* cola_2) que devuelva un array ordenado de mayor a menor con todos los
valores de ambas colas sin repeticiones. Detallar y justificar la complejidad del algoritmo
considerando que el tamaño de las colas es N y M respectivamente.
*/
int* merge_colas(cola_t* cola_1, cola_t* cola_2) {
    pila_t* temp = pila_crear();
    pila_t* repetidos = pila_crear();
    while (!cola_esta_vacia(cola_1) && !cola_esta_vacia(cola_2)) {
        int* dato_1 = cola_ver_primero(cola_1);
        int* dato_2 = cola_ver_primero(cola_2);
        if (*dato_1 < *dato_2) pila_apilar(temp, cola_desencolar(cola_1));
        if (*dato_2 < *dato_1) pila_apilar(temp, cola_desencolar(cola_2));
        if (*dato_1 == *dato_2) {
            pila_apilar(temp, cola_desencolar(cola_1));
            pila_apilar(repetidos, cola_desencolar(cola_2));
        }
    }
    int res[20];
    for (int i = 0; !pila_esta_vacia(temp); i++) {
        res[i] = *(int*) pila_desapilar(temp);
    }
    free(cola_1);
    free(cola_2);
    free(temp);
    free(repetidos);
    return res;
}

int main() {
    int uno = 1;
    int dos = 2;
    int tres = 3;
    int cuatro = 3;
    int cinco = 5;
    int seis = 6;

    cola_t* cola_1 = cola_crear();
    cola_t* cola_2 = cola_crear();
    
    cola_encolar(cola_1, &uno);
    cola_encolar(cola_1, &dos);
    cola_encolar(cola_1, &tres);

    cola_encolar(cola_2, &cuatro);
    cola_encolar(cola_2, &cinco);
    cola_encolar(cola_2, &seis);

    int* res = merge_colas(cola_1, cola_2);
    for (int i = 0; i < 20; i++) {
        printf("%d\n", res[i]);
    }
    return 0;
}
