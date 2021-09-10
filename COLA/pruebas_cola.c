#include "cola.h"
#include "pila.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
// FUNCION AUX PARA LA PRUEBA DEL VOLUMEN.
void armar_arreglo(int arreglo[100], int largo) {
  for (int i = 0; i < largo; i++) {
    arreglo[i] = i;
  }
}
//WRAPPER AUXILIAR PARA LA PRUEBA DE DESTRUCCIÓN
void wrapper_destruir_dato(void* elemento) {
    pila_destruir((pila_t*)elemento);
}
/* ******************************************************************
 *                        PRUEBAS ALUMNO                            *
 * *****************************************************************/

//TEST 01
static void crear_cola_empieza_vacia(void) {
    cola_t* cola = cola_crear();
    print_test("TEST 01 - La cola está vacía tras ser creada.", cola_esta_vacia(cola) == true);
    cola_destruir(cola, NULL);
}
//TEST 02
static void invariante_mantenida(void) {
    int uno = 1;
    int dos = 2;
    cola_t* cola = cola_crear();
    print_test("TEST 02 - El frente de la cola es NULL.", cola_ver_primero(cola) == NULL);
    cola_encolar(cola, &uno);
    int* dato1 = cola_ver_primero(cola);
    print_test("TEST 02 - El frente de la cola es 1.", *dato1 == 1);
    cola_encolar(cola, &dos);
    int* dato2 = cola_desencolar(cola);
    print_test("TEST 02 - El elemento desencolado tiene el dato del anterior frente.", *dato2 == uno);
    int* dato3 = cola_ver_primero(cola);
    print_test("TEST 02 - El nuevo frente de la cola es 2.", *dato3 == dos);
    int* dato4 = cola_desencolar(cola);
    print_test("TEST 02 - El elemento desencolado tiene el dato del anterior frente.", *dato4 == dos);
    print_test("TEST 02 - El nuevo frente de la cola es NULL.", cola_ver_primero(cola) == NULL);
    print_test("TEST 02 - La cola nuevamente esta vacía.", cola_esta_vacia(cola) == true);
    cola_destruir(cola, NULL);
}
//TEST 03
static void prueba_volumen(void) {
    cola_t* cola = cola_crear();
    int arr[100];
    //Encolo 100 nodos.
    armar_arreglo(arr, 100);
    for (int i = 0; i < 100; i++) {
        cola_encolar(cola, &arr[i]);
    }
    //Desencolo y chequeo que el dato del nodo desencolado sea el correspondiente.
    while (!cola_esta_vacia(cola)) {
        int* frente = cola_ver_primero(cola);
        int* desencolado = cola_desencolar(cola);
        printf("Frente actual : %d\n", *frente);
        printf("Elemento desencolado : %d\n", *desencolado);
    }
    print_test("TEST 03 - La cola está vacía", cola_esta_vacia(cola));
    cola_destruir(cola, NULL);
}
//TEST 04
static void encolar_null_es_valido(void) {
    cola_t* cola = cola_crear();
    cola_encolar(cola, NULL);
    print_test("TEST 04 - La cola no está vacía tras encolar NULL.", cola_esta_vacia(cola) == false);
    print_test("TEST 04 - El frente de la cola es NULL.", cola_ver_primero(cola) == NULL);
    cola_destruir(cola, NULL);
}
//TEST 05
static void desencolar_y_ver_primero_al_crear_es_invalido(void) {
    cola_t* cola = cola_crear();
    print_test("TEST 05 - Desencolar en cola vacía devuelve NULL.", cola_desencolar(cola) == NULL);
    print_test("TEST 05 - Ver primero en cola vacía devuelve NULL.", cola_ver_primero(cola) == NULL);    
    cola_destruir(cola, NULL);
}
//TEST 06
static void cola_vacia_igual_a_cola_recien_creada(void) {
    char a = 'a';
    char b = 'b';
    char c = 'c';
    cola_t* cola1 = cola_crear();
    cola_encolar(cola1, &a);
    cola_encolar(cola1, &b);
    cola_encolar(cola1, &c);
    cola_desencolar(cola1);
    cola_desencolar(cola1);
    cola_desencolar(cola1);

    cola_t* cola2 = cola_crear();

    print_test("TEST 06 - El frente de la cola1 es igual al de la cola2.", cola_ver_primero(cola1) == cola_ver_primero(cola2));
    print_test("TEST 06 - Ambas colas están vacías.", cola_esta_vacia(cola1) == cola_esta_vacia(cola2));

    cola_destruir(cola1, wrapper_destruir_dato);
    cola_destruir(cola2, wrapper_destruir_dato);
}

//TEST 07
static void prueba_destruccion_pilas(void) {
    //Creo una cola y le encolo las pilas .
    cola_t* cola = cola_crear();
    pila_t* pilas[10];
    
    for (size_t i = 0; i < 10; i++) {
        pilas[i] = pila_crear();
        print_test("TEST 07 - Se encoló correctamente la pila.", cola_encolar(cola, pilas[i]));
    }
    cola_destruir(cola, wrapper_destruir_dato);
}

//TEST 08
static void prueba_destruccion_free(void) {
    cola_t* cola = cola_crear();
    int* numeros = malloc(20 * sizeof(int));
    char* caracteres = malloc(20 * sizeof(char));

    print_test("TEST 08 - Encolo un arreglo el cual requiere memoria dinamica.", cola_encolar(cola, numeros));
    print_test("TEST 08 - Encolo otro arreglo que requiere memoria dinamica.", cola_encolar(cola, caracteres));
    cola_destruir(cola, free);
}
void pruebas_cola_estudiante() {
    //TEST 01
    crear_cola_empieza_vacia();
    //TEST 02
    invariante_mantenida();
    //TEST 03
    prueba_volumen();
    //TEST 04
    encolar_null_es_valido();
    //TEST 05
    desencolar_y_ver_primero_al_crear_es_invalido();
    //TEST 06
    cola_vacia_igual_a_cola_recien_creada();
    //TEST 07
    prueba_destruccion_pilas();
    //TEST 08
    prueba_destruccion_free();
}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_cola_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif