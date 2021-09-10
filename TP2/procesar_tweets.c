#define _POSIX_C_SOURCE 200809L

#include "cms.h"
#include "box.h"
#include "hash.h"
#include "heap.h"
#include "strutil.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void wrapper_box_destruir(void* elemento) {
    box_destruir( (box_t*) elemento);
}

void imprimir_top_K(hash_t* hash, cms_t* cms, size_t header, int K) {
    hash_iter_t* iter = hash_iter_crear(hash);
    if (!iter) {
        hash_destruir(hash);
        cms_destruir(cms);
        return;
    }

    size_t len_boxes = hash_cantidad(hash);
    void** boxes = malloc(sizeof(void*) * len_boxes);            
    size_t i = 0;
    printf("--- %ld\n", header);
    while(!hash_iter_al_final(iter)) {
        char* tag = (char*) hash_iter_ver_actual(iter);
        size_t frec = cms_obtener(cms, (const char*) tag);
        
        boxes[i] = box_crear(frec, tag);
        i++;
        hash_iter_avanzar(iter);
    }
    hash_iter_destruir(iter);

    //Creamos el heap en O(N)
    heap_t* top_K = heap_crear_arr(boxes, len_boxes, boxcmp);

    for (int j = 0; j < K; j++) {
        box_t* desencolada = heap_desencolar(top_K);
        box_imprimir(desencolada);
        box_destruir(desencolada);
        if (heap_esta_vacio(top_K)) break;
    }
    
    //Destruyo las estructuras utilizadas para imprimir
    free(boxes);
    heap_destruir(top_K, wrapper_box_destruir);
}

void procesar(int N, int K){
    //Establecemos las estructuras a utilizar
    char *buffer = NULL;
    size_t capacidad = 0;
    
    cms_t *cms = cms_crear();
    if (!cms) {
        return;
    }

    hash_t* hash_tags;
    hash_tags = hash_crear(NULL);
    if (!hash_tags) {
        cms_destruir(cms);
        return;
    }

    //Contadores para cada linea y cada tanda de K.
    size_t cont = 0;
    size_t header = 1;

    //Comenzamos a leer el archivo
    while (getline(&buffer, &capacidad, stdin) != EOF){
        agregar_fin_de_linea(buffer);
        char **tags = split(buffer, ',');
        if (!tags) continue;

        //Al leer un tag , lo guardamos en un hash sin valor para no perderlo y se aumenta en CMS las respectivas posiciones.
        for (size_t i = 1; tags[i]; i++) {
            cms_guardar(cms, tags[i]);
            if (!hash_pertenece(hash_tags, tags[i])){
                hash_guardar(hash_tags, tags[i], NULL);
            }
        }
        free_strv(tags);
        cont++;
        //Al llegar a las N lineas debemos empezar el proceso para imprimir los K K
        if (cont % N == 0) {
            //Recorremos los tags , a cada uno le obtengo su minima frecuencia y hago uso
            //de la estructura auxiliar, para que el heap se contruya en O(N) simlemente
            //pedimos memoria para un arreglo el cual sea de un largo igual a los tags leidos, osea
            // el largo del hash.
            imprimir_top_K(hash_tags, cms, header, K);
            hash_destruir(hash_tags);

            //Creo el hash para la próxima lectura de lineas.            
            hash_tags = hash_crear(NULL);
            if (!hash_tags) {
                cms_destruir(cms);
                return;
            }
            header++;
        }
    }
    // Al terminar de leer las lineas, si el hash remanente de la ultima lectura
    // tiene TT's entonces repito la operación del condicional anterior, en caso 
    // contrario simplemente lo destruyo.
    if (hash_cantidad(hash_tags) > 0) {
        imprimir_top_K(hash_tags, cms, header, K);
    }
    hash_destruir(hash_tags);
    //Libero la memoria restante
    free(buffer);
    cms_destruir(cms);
}

int main(int argc, char *vector[]){
    if (argc != 3){
        printf("Cant de argumentos invalidos\n");
        return -1;
    }

    int N = atoi(vector[1]);
    int K = atoi(vector[2]);

    procesar(N, K);
    return 0;
}

// gcc -g -std=c99 -Wall -Wtype-limits -pedantic -Wconversion -Wno-sign-conversion -o procesar_tweets *.c