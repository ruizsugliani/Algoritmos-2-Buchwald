#ifndef LISTA_H
#define LISTA_H
#include <stddef.h>
#include <stdbool.h>

/* ******************************************************************
 *                DEFINICION DE LOS TIPOS DE DATOS
 * *****************************************************************/

/* La lista está planteada como una lista de punteros genéricos. */
struct lista;
typedef struct lista lista_t;

struct lista_iter;
typedef struct lista_iter lista_iter_t;


/* ******************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

//Crea una lista enlazada.
//Post devuelve una nueva lista vacía.
lista_t *lista_crear(void);

// Devuelve verdadero si la lista no tiene elementos enlazados, false en caso contrario.
// Pre: la lista fue creada.
bool lista_esta_vacia(const lista_t *lista);

//Inserta un nodo con el dato pasado por parámetro al principio de la lista.
//Pre: la lista fue creada.
bool lista_insertar_primero(lista_t *lista, void *dato);

//Inserta un nodo con el dato pasado por parámetro al final de la lista.
//Pre: la lista fue creada
bool lista_insertar_ultimo(lista_t *lista, void *dato);

//Borra el primer elemento de la lista, si la lista está vacía se devuelve NULL.
//Pre: la lista fue creada.
//Post: se devolvió el dato del elemento borrado.
void *lista_borrar_primero(lista_t *lista);

// Obtiene el valor del primer elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del primero, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el primer elemento de la lista, cuando no está vacía.
void *lista_ver_primero(const lista_t *lista);

// Obtiene el valor del ultimo elemento de la lista. Si la lista tiene
// elementos, se devuelve el valor del ultimo, si está vacía devuelve NULL.
// Pre: la lista fue creada.
// Post: se devolvió el ultimo elemento de la lista, cuando no está vacía.
void *lista_ver_ultimo(const lista_t* lista);

//Obtiene el largo de la lista.
//Pre: la lista fue creada.
//Post: se devuelve el largo de la lista.
size_t lista_largo(const lista_t *lista);

// Destruye la lista. Si se recibe la función destruir_dato por parámetro,
// para cada uno de los elementos de la lista llama a destruir_dato.
// Pre: la lista fue creada. destruir_dato es una función capaz de destruir
// los datos de la lista, o NULL en caso de que no se la utilice.
// Post: se eliminaron todos los elementos de la lista.
void lista_destruir(lista_t *lista, void (*destruir_dato)(void *));

/* ******************************************************************
 *                    PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/

//Recibe la lista creada, una función visitar y un extra.
//Mientras la función visitar evalúe true se seguirá iterando la lista hasta que devuelva false o el iterador haya pasado por todos los nodos.
//Pre: la lista fue creada y la función visitar devuelve un booleano.
void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra);

/* ******************************************************************
 *                    PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

//Crea un iterador externo.
//La lista está creada.
//Post devuelve al iterador externo que apunta al primer elemento de la lista.
lista_iter_t *lista_iter_crear(lista_t *lista);

//Avanza el iterador al proximo nodo, devuelve un booleando en caso de haber o no un siguiente nodo.
//Pre: la lista (no vacía) y el iterador fueron creados.
//Post: se avanza al proximo nodo.
bool lista_iter_avanzar(lista_iter_t *iter);

//Obtiene el valor del nodo actual al que apunta el iterador y lo devuelve, si la lista está vacía devuelve NULL;
//Pre: La lista (no vacía) y el iterador fueron creados.
//Post: Se devolvió el valor del nodo.
void *lista_iter_ver_actual(const lista_iter_t *iter);

//Devuelve true si está al final de la lista o false en caso contrario
//Pre: la lista y el iterador fueron creados.
bool lista_iter_al_final(const lista_iter_t *iter);

//Destruye el iterador.
//Pre: el iterador fue creado.
void lista_iter_destruir(lista_iter_t *iter);

//Devuelve false en caso de error en la creación del nuevo nodo y true si se completó la insersión de dicho nodo.
//Pre: la lista y el iterador fueron creados.
//Post: el elemento insertado va a tomar la posicion del elemento al que se apunta y el iterador va a apuntar al nuevo elemento.
bool lista_iter_insertar(lista_iter_t *iter, void *dato);

//Elimina el nodo al cual apunta el iterador y devuelve el dato de dicho nodo.
//Pre: la lista y el iterador fueron creados.
//Post: devuelve el dato del nodo eliminado.
void *lista_iter_borrar(lista_iter_t *iter);


/* *****************************************************************
 *                      PRUEBAS UNITARIAS
 * *****************************************************************/
// Realiza pruebas sobre la implementación realizada.
//
// Las pruebas deben emplazarse en el archivo ‘pruebas_lista.c’, y
// solamente pueden emplear la interfaz pública tal y como aparece en lista.h
// (esto es, las pruebas no pueden acceder a los miembros del struct lista).
//
// Para la implementación de las pruebas se debe emplear la función
// print_test(), como se ha visto en TPs anteriores.
void pruebas_lista_estudiante(void);

#endif   //LISTA.H

//gcc -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -o pruebas *.c
//valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./pruebas