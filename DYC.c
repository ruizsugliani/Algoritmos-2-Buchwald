#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
//PARCIALITO
/*
Implementar una función int mas_cercano(int arreglo[], size_t largo, int n) que
reciba un arreglo ordenado de enteros, su largo y un entero N, y devuelva el elemento
del arreglo más cercano a N usando la técnica de división y conquista. ¿Cuál es el orden
del algoritmo? Justificar utilizando el Teorema del Maestro. Ejemplos:
mas_cercano([1, 2, 3, 4, 7], 5, 8) --> 7
mas_cercano([1, 2, 3, 4, 7], 5, 3) --> 3
mas_cercano([1, 2, 3, 4, 7], 5, 0) --> 1
*/
int _mas_cercano(int arreglo[], size_t ini, size_t fin, int n, size_t largo) {
    size_t medio = (ini + fin) / 2;
    if (arreglo[medio] == n || arreglo[medio] - 1 == n || arreglo[medio] + 1 == n) return arreglo[medio];
    if (arreglo[medio] < n ) return _mas_cercano(arreglo, medio + 1, fin, n, largo);
    if (arreglo[medio] > n ) return _mas_cercano(arreglo, 0, medio, n, largo);
}
int mas_cercano(int arreglo[], size_t largo, int n) {
    return _mas_cercano(arreglo, 0, largo - 1, n, largo);
}

int main() {
    int arr[] = {1, 3, 5, 7, 9};
    printf("%d\n", mas_cercano(arr, 5, 11));
    return 0;
}

//BUSCAR MINIMO DYC
int _buscar_minimo(int arr[], int inicio, int fin) {
    if (inicio == fin) return arr[inicio];
    int medio = (inicio + fin) / 2;
    int menor_izq = _buscar_minimo(arr, inicio, medio);
    int menor_der = _buscar_minimo(arr, medio + 1, fin);
    if (menor_izq < menor_der) return menor_izq;
    else return menor_der; 
}

int buscar_minimo(int arr[], int n) {
    return _buscar_minimo(arr, 0, n - 1);
}

//BUSCAR MAXIMO DYC
int buscar_maximo(int arr[], size_t inicio, size_t fin) {
    if (inicio == fin) return arr[inicio];
    size_t medio = (inicio + fin) / 2;
    int lado_izq = buscar_maximo(arr, 0, medio);
    int lado_der = buscar_maximo(arr, medio + 1, fin);
    if (lado_izq > lado_der) return lado_izq;
    else return lado_der;
}

//BUSCAR PICO DYC
size_t posicion_pico(int v[], size_t ini, size_t fin) {
    size_t medio = (ini + fin) / 2;
    if (v[medio - 1] < v[medio] && v[medio] > v[medio + 1]) return medio;
    if (v[medio - 1] < v[medio] && v[medio + 1] > v[medio]) return posicion_pico(v, medio + 1, fin);
    else return posicion_pico(v, 0, medio);
}

//BUSCAR RAIZ CUADRADA DYC
size_t _raiz_entera(size_t n, int ini, int fin) {
    size_t medio = (ini + fin) / 2;
    size_t cuadrado = medio * medio;
    if (cuadrado <= n && (medio + 1) * (medio + 1) > n) return medio;
    if (cuadrado > n) return _raiz_entera(n, 0, medio - 1);
    if (cuadrado < n) return _raiz_entera(n, medio + 1, fin);
}

size_t raiz_entera(size_t n) {
    return _raiz_entera(n, 0, n - 1);
}

//BUSCAR PRIMER CERO DYC
int _primer_cero(int arr[], int ini, int fin) {
    int medio = (ini + fin) / 2;
    if (arr[medio] == 0 && arr[medio - 1] == 1) return medio;
    if (arr[medio] == 1) return _primer_cero(arr, medio + 1, fin);
    return _primer_cero(arr, 0, medio - 1);
}

int primer_cero(int arr[], size_t n) {
    if (arr[n-1] == 1) return -1;
    return _primer_cero(arr, 0, n - 1);
}
//00
bool _arreglo_es_magico(int arr[], size_t inicio, size_t fin);

bool arreglo_es_magico(int arr[], size_t n) {
    return _arreglo_es_magico(arr, 0, n - 1);
}

bool _arreglo_es_magico(int arr[], size_t inicio, size_t fin) {
    size_t medio = (inicio + fin) / 2;
    if (inicio > fin || medio == 0) {
      return false;
    }

    if (arr[medio] == medio) {
      return true;
    }
    if (arr[medio] < medio) {
      return _arreglo_es_magico(arr, medio + 1, fin);
    }
    if (arr[medio] > medio) {
      return _arreglo_es_magico(arr, inicio, medio - 1);
    }
}

//09
bool _arreglo_ordenado(int arr[], size_t ini, size_t fin) {
    if(ini==fin)
        return true;
    
    size_t med = (ini+fin)/2;

    if(arr[med] > arr[med+1])
        return false;

    bool izq = _arreglo_ordenado(arr, ini, med);
    bool der = _arreglo_ordenado(arr, med+1, fin);

    if (izq && der)
        return false;
    return true;
}

bool arreglo_ordenado(int arr[], size_t n) {
    if (n == 0)
        return 1;
    return _arreglo_ordenado(arr, 0, n-1);
}

//10
int _elemento_desordenado(int arr[], size_t inicio, size_t fin) {
    if (inicio == fin) return arr[inicio];
    size_t medio = (inicio + fin) / 2;
    int lado_izq = _elemento_desordenado(arr, 0, medio);
    int lado_der = _elemento_desordenado(arr, medio + 1, fin);
    if (lado_izq > lado_der) return lado_izq;
}

int elemento_desordenado(int arr[], size_t n) {
    return _elemento_desordenado(arr, 0, n - 1);
}
/*
//11
int _primer_cero(int arr[], size_t inicio, size_t fin){
    size_t medio = (fin + inicio) / 2;
    if (arr[fin] == 1) return -1;
    if (inicio == fin) return medio;
    if (arr[medio] == 0) return _primer_cero(arr, 0, medio);
    return _primer_cero(arr, medio + 1, fin);
}

int primer_cero(int arr[], size_t n){
    return _primer_cero(arr, 0, n - 1);
}
*/
//17
int _mas_de_mitad(int* arr, size_t inicio, size_t fin) {
    if (inicio == fin) return arr[inicio];

    size_t medio = (inicio + fin) / 2;
    int candidato1 = _mas_de_mitad(arr, 0, medio);
    int candidato2 = _mas_de_mitad(arr, medio + 1, fin);

    int contador1 = 0, contador2 = 0;
    for (size_t i = inicio; i < fin; i++) {
        if (arr[i] == candidato1) contador1++;
        else if (arr[i] == candidato2) contador2++;
    }
    if (contador1 > contador2) return candidato1;
    else return candidato2;
}

bool mas_de_mitad(int* arr, size_t n) {
    int candidato =_mas_de_mitad(arr, 0, n - 1);
    int contador = 0;
        for (size_t i = 0; i < n; i++) {
            if (arr[i] == candidato) contador++;
        }
    return contador > (n / 2);
}
