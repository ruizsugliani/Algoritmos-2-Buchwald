#include <stdbool.h>
#include <stdio.h>

bool distancia_exacta(int arr[], int k){
    int pos_1 = 0;
    int pos_2 = k - 1;

    int* puntero1 = &arr[pos_1];
    int* puntero2 = &arr[pos_2];
    
    while (arr[pos_2]) {
        if (*puntero1 - *puntero2 == k || *puntero2 - *puntero1 == k) {
            return true;
        }
        puntero1 = &arr[pos_1++];
        puntero2 = &arr[pos_2++];
    }
    return false;
}
//El orden es O(n) ya que se recorre el arreglo una unica vez , mantienendo 2 punteros a distancia k y chequeand
//en cada caso con operaciones O(1) si se cumple lo pedido.

/*
Primitiva para el ABB que nos permite obtener el segundo máximo elemento del ABB, 
en caso de tener menos de dos elementos, devolver NULL.
La primitiva debe ejecutar en O(log n).
*/
const char* _abb_segundo_maximo(const abb_t* abb, abb_nodo_t* nodo, char* clave) {
    if (nodo->der) {
        _abb_segundo_maximo(abb, nodo->der, nodo->clave);
    } else {
        return clave;
    }
}

const char* abb_segundo_maximo(const abb_t* abb) {
    if (abb->cantidad < 2) return NULL;
    return _abb_segundo_maximo(abb, abb->raiz, abb->raiz->clave);
}