#include "cola.h"
#include "pila.h"
#include <stdio.h>
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
        if (dato_1 < dato_2) pila_apilar(temp, cola_desencolar(cola_1));
        if (dato_2 < dato_1) pila_apilar(temp, cola_desencolar(cola_2));
        if (dato_1 == dato_2) {
            pila_apilar(temp, cola_desencolar(cola_1));
            pila_apilar(repetidos, cola_desencolar(cola_2));
        }
    }
    while (!pila_esta_vacia(temp)) {
        printf("%d\n", *(int*) pila_desapilar);
    }
}

/*
Implementar una función int mas_cercano(int arreglo[], size_t largo, int n) que
reciba un arreglo ordenado de enteros, su largo y un entero N, y devuelva el elemento
del arreglo más cercano a N usando la técnica de división y conquista. ¿Cuál es el orden
del algoritmo? Justificar utilizando el Teorema del Maestro. Ejemplos:
mas_cercano([1, 2, 3, 4, 7], 5, 8) --> 7
mas_cercano([1, 2, 3, 4, 7], 5, 3) --> 3
mas_cercano([1, 2, 3, 4, 7], 5, 0) --> 1
*/

/*
Implementar una función lista_t* top_3(lista_t* lista) que reciba una lista enlazada
de enteros y devuelva una lista enlazada nueva con los 3 enteros más grandes. La lista
original no debe ser modificada. Detallar y justificar la complejidad del algoritmo.
top_3([2, 13, 8, 4, 2, 7]) --> [13, 8, 7]
top_3([13, 13, 8, 4, 2, 7]) --> [13, 8, 7]
top_3([13, 13]) --> [13]
*/