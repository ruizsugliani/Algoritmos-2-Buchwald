#include "lista.h"
#include "pila.h"
#include "testing.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

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
static void crear_lista_empieza_vacia(void) {
    printf("----------------------------------- TEST 01 -------------------------------------------\n");
    lista_t* lista = lista_crear();
    print_test("La lista se crea sin elementos.", lista_esta_vacia(lista));
    lista_destruir(lista, NULL);
    printf("\n");

}

//TEST 02
static void insertados_largo_y_borrado_mantenidos(void) {
    printf("----------------------------------- TEST 02 -------------------------------------------\n");
    int uno = 1;
    int dos = 2;
    lista_t* lista = lista_crear();
    print_test("Ver primero devuelve NULL ya que la lista está vacía.", lista_ver_primero(lista) == NULL);
    print_test("El largo de la lista es cero.", lista_largo(lista) == 0);
    lista_insertar_primero(lista, &uno);
    int* primero = lista_ver_primero(lista);
    int* ultimo = lista_ver_ultimo(lista);
    print_test("Ver primero devuelve el dato recién insertado al principio.", *primero == 1);
    print_test("Ver ultimo devuelve el dato recién insertado al principio ya que la lista estava vacia.", *ultimo == 1);
    print_test("El largo de la lista es uno.", lista_largo(lista) == 1);
    print_test("La lista no está vacía.", !lista_esta_vacia(lista));
    lista_insertar_ultimo(lista, &dos);
    int* n_ultimo =  lista_ver_ultimo(lista);
    print_test("Ver ultimo devuelve el dato recién insertado al final.", *n_ultimo == 2);
    print_test("El largo de la lista es dos.", lista_largo(lista) == 2);
    int* borrado = lista_borrar_primero(lista);
    print_test("El elemento borrado fue el primer insertado", *borrado == 1);
    print_test("El largo de la lista vuelve a ser 1.", lista_largo(lista) == 1);
    int* n_borrado = lista_borrar_primero(lista);
    print_test("El elemento borrado fue el segundo insertado", *n_borrado == 2);
    print_test("El largo de la lista vuelve a ser 0.", lista_largo(lista) == 0);
    print_test("La lista vuelva a estar vacía.", lista_esta_vacia(lista));
    lista_destruir(lista, NULL);
    printf("\n");
}

//TEST 03
static void prueba_volumen(void) {
    printf("----------------------------------- TEST 03 -------------------------------------------\n");
    lista_t* lista = lista_crear();
    int arr[100];
    //Encolo 100 nodos.
    armar_arreglo(arr, 100);
    for (int i = 0; i < 100; i++) {
        lista_insertar_primero(lista, &arr[i]);
    }
    //Borro el primero y chequeo que el dato del nodo borrado sea el correspondiente.
    size_t cont = 99;
    while (!lista_esta_vacia(lista)) {
        int* primero = lista_ver_primero(lista);
        lista_borrar_primero(lista);
        print_test("El primero es el siguiente : \n", *primero == cont);
        cont--;
    }
    print_test("La lista está vacía", lista_esta_vacia(lista));
    lista_destruir(lista, NULL);
    printf("\n");
}

//TEST 04
static void insertar_null_es_valido(void) {
    printf("----------------------------------- TEST 04 -------------------------------------------\n");
    lista_t* lista = lista_crear();
    lista_insertar_primero(lista, NULL);
    print_test("La lista no está vacía tras insertar el principio es NULL.", !lista_esta_vacia(lista));
    print_test("El primero de la lista es NULL.", lista_ver_primero(lista) == NULL);
    print_test("El ultimo de la lista es NULL.", lista_ver_ultimo(lista) == NULL);
    lista_insertar_ultimo(lista, NULL);
    print_test("El ultimo de la lista tras insertar NULL al final es NULL.", lista_ver_ultimo(lista) == NULL);
    lista_destruir(lista, NULL);
    printf("\n");
}

//TEST 05
static void borrar_ver_primero_y_ver_ultimo_al_crear_es_invalido(void) {
    printf("----------------------------------- TEST 05 -------------------------------------------\n");
    lista_t* lista = lista_crear();
    print_test("Borrar primero en lista vacía devuelve NULL.", lista_borrar_primero(lista) == NULL);
    print_test("Ver primero en lista vacía devuelve NULL.", lista_ver_primero(lista) == NULL);
    print_test("Ver ultimo en lista vacía devuelve NULL.", lista_ver_ultimo(lista) == NULL);    
    lista_destruir(lista, NULL);
    printf("\n");
}

//TEST 06
static void lista_vacia_igual_a_lista_recien_creada(void) {
    printf("----------------------------------- TEST 06 -------------------------------------------\n");
    char a = 'a';
    char b = 'b';
    char c = 'c';
    lista_t* lista1 = lista_crear();
    
    lista_insertar_primero(lista1, &a);
    lista_insertar_primero(lista1, &b);
    lista_insertar_primero(lista1, &c);
    lista_borrar_primero(lista1);
    lista_borrar_primero(lista1);
    lista_borrar_primero(lista1);

    lista_t* lista2 = lista_crear();

    print_test("El primero de la lista1 es igual al de la lista2.", lista_ver_primero(lista1) == lista_ver_primero(lista2));
    print_test("Ambas listas están vacías.", lista_esta_vacia(lista1) == lista_esta_vacia(lista2));

    lista_destruir(lista1, NULL);
    lista_destruir(lista2, NULL);
    printf("\n");
}

//TEST 07
static void prueba_destruccion_pilas(void) {
    printf("----------------------------------- TEST 07 -------------------------------------------\n");
    //Creo una lista y le encolo las pilas .
    lista_t* lista = lista_crear();
    pila_t* pilas[10];
    
    for (size_t i = 0; i < 10; i++) {
        pilas[i] = pila_crear();
        print_test("Se encoló correctamente la pila.", lista_insertar_primero(lista, pilas[i]));
    }
    lista_destruir(lista, wrapper_destruir_dato);
    printf("\n");
}

//TEST 08
static void prueba_destruccion_free(void) {
    printf("----------------------------------- TEST 08 -------------------------------------------\n");
    lista_t* lista = lista_crear();
    int* numeros = malloc(20 * sizeof(int));
    char* caracteres = malloc(20 * sizeof(char));

    print_test("Inserto al principio un arreglo el cual requiere memoria dinamica.", lista_insertar_primero(lista, numeros));
    print_test("Inserto al principio otro arreglo que requiere memoria dinamica.", lista_insertar_primero(lista, caracteres));
    lista_destruir(lista, free);
    printf("\n");
}

//TEST 09
//FUNCION VISITAR QUE IMPRIME LOS DATOS DE LA LISTA.
bool visitar_imprime_3_datos(void* dato, void* extra) {
    if (*(int*) extra == 3) return false;
    printf("%d\n", *(int*) dato);
    *(int*) extra +=1;
    return true;
}
static void extra_con_iter_interno_es_condicion_de_corte(void) {
    printf("----------------------------------- TEST 09 -------------------------------------------\n");
    lista_t* lista = lista_crear();
    int tres = 3;
    int cuatro = 4;
    int cinco = 5;
    int seis = 6;
    int siete = 7;
    int ocho = 8; 
    lista_insertar_primero(lista, &seis);
    lista_insertar_primero(lista, &cinco);
    lista_insertar_primero(lista, &cuatro);
    lista_insertar_primero(lista, &tres);
    lista_insertar_ultimo(lista, &siete);
    lista_insertar_ultimo(lista, &ocho);
    int cont = 0;
    lista_iterar(lista, visitar_imprime_3_datos, &cont);
    print_test("Se imprimieron 3 datos con el iterador.", cont == 3);
    lista_destruir(lista, NULL);
    printf("\n");
}

//TEST 10
//FUNCION VISITAR QUE SUMA LOS DATOS DE LA LISTA
bool visitar_suma(void* dato, void* extra) {
    *(int*) extra += *(int*) dato;
    return true;
}
static void extra_con_iter_interno_suma_y_recorre_toda_la_lista(void) {
    printf("----------------------------------- TEST 10 -------------------------------------------\n");
    lista_t* lista = lista_crear();
    int uno = 1;
    int dos = 2;
    int tres = 3;
    int res = 0;
    lista_insertar_primero(lista, &tres);
    lista_insertar_primero(lista, &dos);
    lista_insertar_primero(lista, &uno);
    lista_iterar(lista, visitar_suma, &res);
    print_test("La suma de los datos con el iterador interno es 6.", res == 6);
    lista_destruir(lista, NULL);
}

//TEST 11
static void insertar_al_principio_iter_externo(void) {
    printf("----------------------------------- TEST 11 -------------------------------------------\n");
    int uno = 1;
    lista_t* lista = lista_crear();
    lista_iter_t* iter = lista_iter_crear(lista);
    lista_iter_insertar(iter, &uno);
    print_test("El iterador apunta al nodo con el dato insertado.", *(int*) lista_iter_ver_actual(iter) == 1);
    print_test("El largo de la lista es el correcto.", lista_largo(lista) == 1);
    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
}

//TEST 12
static void insertar_en_el_medio_iter_externo(void) {
    printf("----------------------------------- TEST 12 -------------------------------------------\n");
    int cero = 0;
    int uno = 1;
    int dos = 2;
    int tres = 3;
    lista_t* lista = lista_crear();
    lista_insertar_primero(lista, &tres);
    lista_insertar_primero(lista, &dos);
    lista_insertar_primero(lista, &cero);
    lista_iter_t* iter = lista_iter_crear(lista);
    lista_iter_avanzar(iter);
    print_test("El iterador apunta al nodo del medio.", *(int*) lista_iter_ver_actual(iter) == 2);
    print_test("El largo de la lista es el correcto.", lista_largo(lista) == 3);
    lista_iter_insertar(iter, &uno);
    print_test("El iterador apunta al nodo con el dato insertado.", *(int*) lista_iter_ver_actual(iter) == 1);
    print_test("El largo de la lista es el correcto.", lista_largo(lista) == 4);
    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
    printf("\n");
}

//TEST 13
static void insertar_al_final_iter_externo(void) {
    printf("----------------------------------- TEST 13 -------------------------------------------\n");
    int cero = 0;
    int uno = 1;
    int dos = 2;
    int tres = 3;
    lista_t* lista = lista_crear();
    lista_insertar_primero(lista, &dos);
    lista_insertar_primero(lista, &uno);
    lista_insertar_primero(lista, &cero);
    lista_iter_t* iter = lista_iter_crear(lista);
    lista_iter_avanzar(iter);
    lista_iter_avanzar(iter);
    print_test("El iterador apunta al ultimo nodo.", 2 == *(int*) lista_iter_ver_actual(iter));
    print_test("El largo de la lista es el correcto.", lista_largo(lista) == 3);
    //Avanzo nuevamente para que el iter apunte a NULL.
    lista_iter_avanzar(iter);
    lista_iter_insertar(iter, &tres);
    print_test("El iterador apunta al nodo con el dato insertado.", *(int*) lista_iter_ver_actual(iter) == 3);
    print_test("El largo de la lista es el correcto.", lista_largo(lista) == 4);
    
    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
    printf("\n");
}
//TEST 14
static void borrar_al_principio_iter_externo(void) {
    printf("----------------------------------- TEST 14 -------------------------------------------\n");
    int uno = 1;
    int dos = 2;
    int tres = 3;

    lista_t* lista = lista_crear();

    lista_insertar_primero(lista, &tres);
    lista_insertar_primero(lista, &dos);
    lista_insertar_primero(lista, &uno);
    //Queda una lista de la forma: [1] -> [2] -> [3]
    lista_iter_t* iter = lista_iter_crear(lista);
    //No avanzo al iterador así apunta al primer nodo de la lista.
    print_test("Corroboro que el iterador apunta al nodo del principio.", 1 == *(int*) lista_iter_ver_actual(iter));
    print_test("El largo de la lista es el correcto (largo 3).", lista_largo(lista) == 3);
    int* dato = lista_iter_borrar(iter);
    print_test("El nodo eliminado contenia al 1 como dato.", *dato == 1);
    print_test("Corroboro que el iterador apunta al nodo proximo al anterior.", 2 == *(int*) lista_iter_ver_actual(iter));
    print_test("El largo de la lista es el correcto (largo 2).", lista_largo(lista) == 2);
    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
    printf("\n");
}
//TEST 15
static void borrar_en_el_medio_iter_externo(void) {
    printf("----------------------------------- TEST 15 -------------------------------------------\n");
    int uno = 1;
    int dos = 2;
    int tres = 3;

    lista_t* lista = lista_crear();

    lista_insertar_primero(lista, &tres);
    lista_insertar_primero(lista, &dos);
    lista_insertar_primero(lista, &uno);
    //Queda una lista de la forma: [1] -> [2] -> [3].
    lista_iter_t* iter = lista_iter_crear(lista);
    lista_iter_avanzar(iter);
    print_test("Corroboro que el iterador apunta al nodo del medio.", 2 == *(int*) lista_iter_ver_actual(iter));
    print_test("El largo de la lista es el correcto (largo 3).", lista_largo(lista) == 3);
    int* dato = lista_iter_borrar(iter);
    print_test("El nodo eliminado contenia al 2 como dato.", *dato == 2);
    print_test("Corroboro que el iterador apunta al nodo proximo al anterior.", 3 == *(int*) lista_iter_ver_actual(iter));
    print_test("El largo de la lista es el correcto (largo 2).", lista_largo(lista) == 2);
    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
    printf("\n");
}

//TEST 16
static void borrar_al_final_iter_externo(void) {
    printf("----------------------------------- TEST 16 -------------------------------------------\n");
    int uno = 1;
    int dos = 2;
    int tres = 3;

    lista_t* lista = lista_crear();

    lista_insertar_primero(lista, &tres);
    lista_insertar_primero(lista, &dos);
    lista_insertar_primero(lista, &uno);
    //Queda una lista de la forma: [1] -> [2] -> [3].
    lista_iter_t* iter = lista_iter_crear(lista);
    lista_iter_avanzar(iter);
    lista_iter_avanzar(iter);
    print_test("Corroboro que el iterador apunta al nodo del final.", *(int*) lista_ver_ultimo(lista) == *(int*) lista_iter_ver_actual(iter));
    print_test("El largo de la lista es el correcto (largo 3).", lista_largo(lista) == 3);
    int* dato = lista_iter_borrar(iter);
    print_test("El nodo eliminado contenia al 3 como dato.", *dato == 3);
    print_test("Corroboro que el iterador apunta al nodo proximo al anterior.", lista_iter_al_final(iter));
    print_test("El largo de la lista es el correcto (largo 2).", lista_largo(lista) == 2);
    lista_iter_destruir(iter);
    lista_destruir(lista, NULL);
    printf("\n");
}

void pruebas_lista_estudiante() {
    //TEST 01
    crear_lista_empieza_vacia();
    //TEST 02
    insertados_largo_y_borrado_mantenidos();
    //TEST 03
    prueba_volumen();
    //TEST 04
    insertar_null_es_valido();
    //TEST 05
    borrar_ver_primero_y_ver_ultimo_al_crear_es_invalido();
    //TEST 06
    lista_vacia_igual_a_lista_recien_creada();
    //TEST 07
    prueba_destruccion_pilas();
    //TEST 08
    prueba_destruccion_free();
    //TEST 09
    extra_con_iter_interno_es_condicion_de_corte();
    //TEST 10
    extra_con_iter_interno_suma_y_recorre_toda_la_lista();
    //TEST 11
    insertar_al_principio_iter_externo();
    //TEST 12
    insertar_en_el_medio_iter_externo();
    //TEST 13
    insertar_al_final_iter_externo();
    //TEST 14
    borrar_al_principio_iter_externo();
    //TEST 15
    borrar_en_el_medio_iter_externo();
    //TEST 16
    borrar_al_final_iter_externo();
}


/*
 * Función main() que llama a la función de pruebas.
 */

#ifndef CORRECTOR  // Para que no dé conflicto con el main() del corrector.

int main(void) {
    pruebas_lista_estudiante();
    return failure_count() > 0;  // Indica si falló alguna prueba.
}

#endif
//gcc -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -o pruebas *.c
//valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas