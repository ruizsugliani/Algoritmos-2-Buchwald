#include "pila.h"
#include <stdlib.h>
#include <stdio.h>
#define TAM_MIN 10
/* Definición del struct pila proporcionado por la cátedra.
 */
struct pila {
    void **datos;
    size_t cantidad;   // Cantidad de elementos almacenados.
    size_t capacidad;  // Capacidad del arregloeglo 'datos'.
};

/* *****************************************************************
 *                    PRIMITIVAS DE LA PILA
 * *****************************************************************/

pila_t *pila_crear(void) {
    pila_t *pila= malloc(sizeof(pila_t));
    if (pila == NULL) {
        return NULL;
    }
    pila->datos = malloc(TAM_MIN * sizeof(void*));
    if (pila->datos == NULL) {
        free(pila);
        return NULL;
    }
    pila->cantidad = 0;
    pila->capacidad = TAM_MIN;
    return pila;
}

void pila_destruir(pila_t *pila) {
    free(pila->datos);
    free(pila);
}

bool pila_esta_vacia(const pila_t *pila) {
    if (pila->cantidad == 0) {
        return true;
    } else {
        return false;
    }
}

void *pila_ver_tope(const pila_t *pila) {
    if (pila_esta_vacia(pila)) {
        return NULL;
    }
    return pila->datos[pila->cantidad - 1];
}

bool pila_redimensionar(pila_t *pila, size_t tam_nuevo) {
    if (tam_nuevo < TAM_MIN) {
        tam_nuevo = TAM_MIN;
    }
    void** aux = realloc(pila->datos, sizeof(void*) * tam_nuevo);
    if (aux == NULL) {
        return false;
    }
    pila->datos = aux;
    pila->capacidad = tam_nuevo;
    return true;
}

bool pila_apilar(pila_t *pila, void *valor) {
    if (pila->cantidad == pila->capacidad) {
        pila_redimensionar(pila, pila->capacidad * 2);
    }
    pila->datos[pila->cantidad] = valor;
    pila->cantidad++;
    return true;       
}

void *pila_desapilar(pila_t *pila) {
    if (pila_esta_vacia(pila)) {
        return NULL;
    }
    void* tope = pila_ver_tope(pila);
    pila->cantidad--;
    if ((pila->capacidad > TAM_MIN) && (pila->cantidad * 4 <= pila->capacidad)) {
        pila_redimensionar(pila, pila->capacidad / 2);
    }
    return tope;
}


//EJERCICIOS DE PILAS

void invertir_arreglo(void** arreglo, size_t n) {
    pila_t* pila = pila_crear();
    for (int i = 0; i < n; i++) {
        pila_apilar(pila, arreglo[i]);
    }
    for (int i = 0; !pila_esta_vacia(pila); i++) {
        arreglo[i] = pila_desapilar(pila);
    }
}

bool pila_es_piramidal(pila_t* pila) {
    pila_t* aux = pila_crear();
    bool piramidal = true;
    while (!pila_esta_vacia(pila) && piramidal) {
        int* dato = (int*) pila_desapilar(pila);
        int* tope = (int*) pila_ver_tope(pila);

        piramidal = tope ? *dato < *tope : true;
        
        pila_apilar(aux, (void*) dato); 
    }
    while (!pila_esta_vacia(aux)) {
        pila_apilar(pila, pila_desapilar(aux));
    }
    pila_destruir(aux);
    return piramidal;
}

//La complejidad de la primitiva es O(N) ya que recorre el arreglo dinamico
//de la pila una vez y hace operaciones O(1) en el proceso como guardar en el 
//arreglo resultante o comparar si esta en el rango de posiciones donde hacer lo anterior.
void** pila_medio(const pila_t* pila, size_t k) {
    size_t pos_inicio = (pila->cantidad / 2) - 1;
    size_t pos_final = pos_inicio + k - 1;
    size_t pos_arr = k - 1;

    void** res = malloc(sizeof(void*) * k);
    if (!res) return NULL;
    //Por si hay menos de K elementos en la pila , primero guardo NULL en todas las posiciones.
    //El cual a lo sumo es O(n) y por ende la complejidad sigue siendo la misma.
    for (size_t j = 0; j < k - 1; j++) {
        res[j] = NULL;
    }
    for (size_t i = 0; i < pila->cantidad; i++) {
        if (i >= pos_inicio && i <= pos_final) {
            res[pos_arr] = pila->datos[i];
            pos_arr--;
        }
    }
    return res;
}
int main() {
    pila_t* pila = pila_crear();
    int uno = 1;
    int dos = 2;
    int tres = 3;
    int cuatro = 4;
    int cinco = 5;
    int seis = 6;
    int siete = 7;
    int ocho = 8;
    int nueve = 9;
    
    pila_apilar(pila, &uno);
    pila_apilar(pila, &dos);
    pila_apilar(pila, &tres);
    pila_apilar(pila, &cuatro);
    pila_apilar(pila, &cinco);
    //#pila_apilar(pila, &seis);
    //pila_apilar(pila, &siete);
    //pila_apilar(pila, &ocho);
    //pila_apilar(pila, &nueve);

    void** res = pila_medio(pila, 2);
    for (int i = 0; i < 2; i++) {
        if (res[i]) printf("%d\n", *(int*) res[i]);
        else printf("NULL\n");
    }
    printf("-----------\n");
    while (!pila_esta_vacia(pila)) {
        printf("%d\n", *(int*) pila_desapilar(pila));
    }
    free(res);
    pila_destruir(pila);
}