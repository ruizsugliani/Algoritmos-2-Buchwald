#define _POSIX_C_SOURCE 200809L

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "strutil.h"
#include "ord_imp.h"
#include "hash.h"
#include "vector.h"
#include "lista.h"

void destruir_hash_wrapper(void *elemento){
    hash_destruir(elemento);
}

void ordenar_e_imprimir(vector_t *cant_tts, hash_t *hash_ocurrencias){
    //Ordenamos las ocurrencias con counting  y luego las recorremos, accediendo a cada una en el hash para intercambiar
    //La lista enlazada por el arreglo que devuelve la nueva primitiva (solo para este tp), poder ordenarlo y finalmente imprimir.
    vector_ordenar(cant_tts);
    
    //RECORRER OCURRENCIAS ORDENADAS Y PARA CADA UNA ORDENAR ARREGLO DE CHAR *E IMPRIMIR
    for (int i = vector_largo(cant_tts) - 1; i >= 0; i--) {
        int num = vector_obtener(cant_tts, i);

        // Si el tam es mayor que 10 pido memoria para almacenar los numeros junto con el /0, sino pido memoria para un numero solo.
        size_t len_tam = num >= 10 ? 3 : 2;

        char clave[len_tam];
        sprintf(clave, "%d/0", num);

        lista_t *lista_actual = hash_obtener(hash_ocurrencias, clave);
        char **nuevo_valor = arr_usuarios(lista_actual);
        int len_nuevo_valor = len_cadenas(nuevo_valor);
        if (len_nuevo_valor > 1) radixsort_chars(nuevo_valor, len_nuevo_valor);
        
        imprimir_cadenas(nuevo_valor, len_nuevo_valor, num);
        
        lista_destruir(lista_actual, NULL);
        free(nuevo_valor);
    }
}

void liberar_memoria(hash_t *hash_usuarios, hash_t *hash_ocurrencias, vector_t *cant_tts, char *linea){
    //Libero toda la memoria pedida
    hash_destruir(hash_usuarios);
    hash_destruir(hash_ocurrencias);
    vector_destruir(cant_tts);
    free(linea);
}

void _procesar_usuarios(hash_t *hash_usuarios, char *usuario, char **vector){
    //Si el usuario no está en el hash lo agrego y su clave será un hash vacío.
        if (!hash_pertenece(hash_usuarios, usuario)){
            hash_t *hash_tts = hash_crear(NULL);
            hash_guardar(hash_usuarios, usuario, hash_tts); 
        }

        hash_t *hash_actual = hash_obtener(hash_usuarios, usuario);

        //Itero el resto de la linea (tts) y si están en el hash perteneciente al usuario los ignoro, en caso contrario los agrego solo como clave.
        for (size_t i = 1; vector[i]; i++){
            if (!hash_pertenece(hash_actual, vector[i])){
                hash_guardar(hash_actual, vector[i], vector[i]);
            }
        }
}

//Prepara las estructuras para poder ordenar los usuarios.
//Hash de ocurrencias con listas enlazadas de usuarios como valores.
void preparar_para_ordenar(vector_t *cant_tts, hash_t *hash_ocurrencias, hash_t *hash_usuarios){
        
    hash_iter_t *hash_iter = hash_iter_crear(hash_usuarios);
    if (!hash_iter) return;

    while (!hash_iter_al_final(hash_iter)){
        const char *clave_actual = hash_iter_ver_actual(hash_iter);
        hash_t *valor_actual = hash_obtener(hash_usuarios, (char*)clave_actual);
        size_t tam = hash_cantidad(valor_actual);

        // Si el tam es mayor o igual que 10 pido memoria para almacenar los numeros junto con el /0, sino pido memoria para un numero solo.
        size_t len_tam = tam >= 10 ? 3 : 2;

        char cantidad_tts_unica[len_tam];
        sprintf(cantidad_tts_unica, "%ld/0", tam);
        
        if (!hash_pertenece(hash_ocurrencias, cantidad_tts_unica)){
            lista_t *usuarios = lista_crear();
            if (!usuarios) return;

            lista_insertar_ultimo(usuarios, (char *)clave_actual);
            hash_guardar(hash_ocurrencias, cantidad_tts_unica, usuarios);
            vector_guardar(cant_tts, (int)tam);

        } else {
            lista_t *usuarios_act = hash_obtener(hash_ocurrencias, cantidad_tts_unica);
            lista_insertar_ultimo(usuarios_act, (char *)clave_actual);
        }

        hash_iter_avanzar(hash_iter);
    }

    hash_iter_destruir(hash_iter);
}

void procesar_usuarios(FILE *archivo){
    char *linea = NULL;
    size_t capacidad = 0;

    //Hash donde las claves son usuarios y los valores son hashes de tts (hash de hashes).
    hash_t *hash_usuarios = hash_crear(destruir_hash_wrapper);

    //Comienzo a leer las lineas del archivo.    
    while (getline(&linea, &capacidad, archivo) != EOF){
        agregar_fin_de_linea(linea);

        char **vector = split(linea, ',');
        if (!vector) continue;

        char *usuario = vector[0];

        _procesar_usuarios(hash_usuarios, usuario, vector);

        free_strv(vector);
    }

    vector_t *cant_tts = vector_crear();
    if (!cant_tts) return;

    //Itero el hash de usuarios y en un hash de ocurrencias (cantidades de tts) guardo la ocurrencia como clave y 
    //su valor es una lista enlazada de nombres de usuarios (donde usamos la nueva primitiva que nos devuelve un arreglo de char *con los usuarios)
    //para luego aplicarle radix y ordenarlo alfabeticamente.
    hash_t *hash_ocurrencias = hash_crear(NULL);
    if (!hash_ocurrencias) return;

    preparar_para_ordenar(cant_tts, hash_ocurrencias, hash_usuarios);

    ordenar_e_imprimir(cant_tts, hash_ocurrencias);

    liberar_memoria(hash_usuarios, hash_ocurrencias, cant_tts, linea);
}


FILE *abrir_archivo(int argc, char *vector[]){
    if (argc < 2){
        printf("parametros insuficientes\n");
        return NULL;
    }

    char *archivo = vector[1];

    FILE *mi_archivo = fopen(archivo, "r");
    if (!mi_archivo){
        printf("No se puedo abrir el archivo\n");
        return NULL;
    }

    return mi_archivo;
}

int main(int argc, char *vector[]){
    
    FILE *mi_archivo = abrir_archivo(argc, vector);
    if (!mi_archivo) return -1;

    procesar_usuarios(mi_archivo);

    fclose(mi_archivo);
    return 0;
}