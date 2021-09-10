#include "pila.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>

void armar_arreglo(int arreglo[100], int largo) {
  for (int i = 0; i < largo; i++) {
    arreglo[i] = i;
  }
}

void imprimir_arreglo(int arreglo[], int largo) {
  for (int i = 0; i < largo; i++) {
    printf("%d\n", arreglo[i]);
  }
}

/* ******************************************************************
 *                        PRUEBAS ALUMNO                            *
 * *****************************************************************/

//1
static void prueba_invariantes(void) {
    int uno = 1;

    pila_t *pila = pila_crear();
    print_test(">>> TEST 01 - La pila está vacía, por lo tanto el tope es NULL.", pila_ver_tope(pila) == NULL);
    pila_apilar(pila, &uno);
    print_test(">>> TEST 01 - La pila no está vacía.", pila_esta_vacia(pila) == false);
    int* tope = pila_ver_tope(pila); 
    print_test(">>> TEST 01 - El tope ya no es NULL, sino que es el valor apilado.", *tope == uno);
    print_test(">>> TEST 01 - Desapilar devuelve el valor anteriormente apilado.", pila_desapilar(pila) == &uno);
    print_test(">>> TEST 01 - La pila nuevamente esta vacía.", pila_esta_vacia(pila) == true);
    print_test(">>> TEST 01 - El tope nuevemente es NULL.", pila_ver_tope(pila) == NULL);
    pila_destruir(pila);
}
//2
static void prueba_volumen(void) {
    pila_t *pila = pila_crear();
    int arreglo[100];
    armar_arreglo(arreglo, 100);
    for (int i = 0; i < 100; i++) {
        pila_apilar(pila, &arreglo[i]);
        int* tope = pila_ver_tope(pila);
        print_test(">>> TEST 02 - El nuevo tope es el valor apilado.", *tope == arreglo[i]);
        print_test(">>> TEST 02 - La pila no está vacía.", pila_esta_vacia(pila) == false);
        printf("Nuevo tope: %d\n", arreglo[i]);
    }
    printf("----------------------------------------------------------------------------------\n");
    for (int j = 100; j > 0; j--) {
        int* tope = pila_ver_tope(pila);
        int* desapilar = pila_desapilar(pila);
        print_test("TEST 02 - El tope es el correcto", *tope == arreglo[j - 1]);
        print_test("TEST 02 - El valor desapilado es el correcto", *desapilar == arreglo[j - 1]);
    }
    print_test("La pila esta vacia", pila_esta_vacia(pila));
    
    pila_destruir(pila);
}
//3
static void prueba_apilar_null(void) {
    pila_t *pila = pila_crear();
    print_test(">>> TEST 03 - El apilamiento del elemento NULL es válido.", pila_apilar(pila, NULL) == true);
    pila_destruir(pila);
}
//4
static void prueba_pila_recien_creada_igual_a_pila_tras_desapilar(void) {
    char a = 'a';
    char b = 'b';

    pila_t *pila = pila_crear();
    bool pila_estado_inicial = pila_esta_vacia(pila);
    
    pila_apilar(pila, &a);
    pila_apilar(pila, &b);
    pila_desapilar(pila);
    pila_desapilar(pila);

    bool pila_estado_final = pila_esta_vacia(pila);

    print_test(">>> TEST 04 - Comprobar que al desapilar hasta que está vacía hace que la pila se comporte como recién creada.", pila_estado_inicial == pila_estado_final);
    pila_destruir(pila);
}

//5
static void prueba_desapilar_y_ver_tope_invalidas(void) {
    pila_t *pila = pila_crear();
    print_test(">>> TEST 05 - La acción de desapilar en una pila recién creada es inválida.", pila_desapilar(pila) == NULL);
    print_test(">>> TEST 05 - La acción de ver_tope en una pila recién creada es inválida.", pila_ver_tope(pila) == NULL);
    pila_destruir(pila);
}

//6
static void prueba_pila_vacia(void) {
    pila_t *pila = pila_crear();
    print_test(">>> TEST 06 - La acción de esta_vacía en una pila recién creada es verdadero.", pila_esta_vacia(pila));
    pila_destruir(pila);
}

//7
static void prueba_apilado_y_desapilado_hasta_tener_pila_vacia(void) {
    pila_t *pila = pila_crear();
    int uno = 1;
    int dos = 2;
    int tres = 3;

    pila_apilar(pila, &uno);
    pila_apilar(pila, &dos);
    pila_apilar(pila, &tres);
    
    pila_desapilar(pila);
    pila_desapilar(pila);
    pila_desapilar(pila);
    
    print_test(">>> TEST 07 - La acción de desapilar en una pila a la que se le apiló y desapiló hasta estar vacía es inválida.", pila_desapilar(pila) == NULL);
    print_test(">>> TEST 07 - La acción de ver_tope en una pila a la que se le apiló y desapiló hasta estar vacía es inválida.", pila_ver_tope(pila) == NULL);
    pila_destruir(pila);
}

void pruebas_pila_estudiante() {
    //1
    prueba_invariantes();
    //2
    prueba_volumen();
    //3
    prueba_apilar_null();
    //4
    prueba_pila_recien_creada_igual_a_pila_tras_desapilar();
    //5
    prueba_desapilar_y_ver_tope_invalidas();
    //6
    prueba_pila_vacia();
    //7
    prueba_apilado_y_desapilado_hasta_tener_pila_vacia();
}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_pila_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
