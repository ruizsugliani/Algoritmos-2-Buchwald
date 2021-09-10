#ifndef BOX_H
#define BOX_H

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <string.h>

//Estructura
typedef struct box box_t;

//Funcion de comparacion de boxes.
int boxcmp(const void* a, const void* b);

//Crea una box con una frecuencia y una cadena por parámetro.
box_t* box_crear(size_t frecuencia, const char* cadena);

//Devuelve la frecuencia asignada a la box
size_t box_frecuencia(box_t* box);

//Devuelve la cadena asignada a la box
const char* box_cadena(box_t* box);

//Imprime "frecuencia cadena" 
void box_imprimir(box_t* box);

//Destruye la box pasada por parámetro.
void box_destruir(box_t* box);

#endif  // BOX_H