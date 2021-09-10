#define _POSIX_C_SOURCE 200809L
#include "strutil.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *substr(const char *str, size_t n) {
    char* nueva_cadena;
    if (n > strlen(str)) {
        nueva_cadena = strdup(str);
        if (!nueva_cadena) return NULL;
        return nueva_cadena;
    }
    nueva_cadena = malloc(sizeof(char) * (n + 1) );
    if (!nueva_cadena) return NULL;
    
    size_t i = 0;
    for(; i < n; i++) {
        nueva_cadena[i] = str[i];
    }
    nueva_cadena[i] = '\0';
    return nueva_cadena;
}

char **split(const char *str, char sep) {
    //Recorro la cadena buscando las apariciones del separador.
    size_t apariciones = 0;
    for (size_t i = 0; i < strlen(str); i++) if (str[i] == sep) apariciones++;
    //Pido memoria para el arreglo dinámico de cadenas.
    size_t len_strv = apariciones + 2;
    char** cadenas = malloc(sizeof(char*) * (len_strv));
    if (!cadenas) return NULL;
    //Recorro nuevamente el arreglo haciendo copias de la cadena a medida que encuentro al sep.
    if (!str || apariciones == 0) {
        cadenas[0] = substr(str, strlen(str));
        cadenas[1] = NULL;
        return cadenas;
    }
    size_t j = 0;
    size_t inicio = 0;
    size_t chars = 0;
    size_t len_str = strlen(str);
    for (size_t i = 0; i < len_str + 1; i++) {
        if (str[i] == sep || str[i] == '\0') {
            cadenas[j] = substr(str + inicio, chars);
            chars = 0;
            inicio = i + 1;
            j++;
        } else {
            chars++;
        }  
    }
    cadenas[j] = NULL;
    return cadenas;
}

char *join(char **strv, char sep) {
    //Caso borde
    if (!strv || strv[0] == NULL) {
        char* cadena = calloc(1, sizeof(char));
        if (!cadena) return NULL;
        return cadena;
    }
    //Recorro el arreglo de cadenas contando la cantidad de letras en total más la longitud de dicho arreglo y luego pido memoria.
    size_t chars = 0, len_strv = 0, len_cadena;
    for (size_t i = 0; strv[i] != NULL; i++) {
        chars += strlen(strv[i]);
        len_strv++;
    }
    len_cadena = chars + len_strv;
    char* cadena = calloc(len_cadena, sizeof(char));
    if (!cadena) return NULL;
    //Recorro el arreglo de cadenas y las uno al mismo tiempo con el separador.
    char* puntero_al_final;
    //Caso generico.
    for (int i = 0; strv[i] != NULL; i++) {
        if (i == 0) {
            puntero_al_final = stpcpy(cadena, strv[i]);
        } else if (sep == '\0') {
            puntero_al_final = stpcpy(puntero_al_final, strv[i]);
            continue;
        } else {
            puntero_al_final = stpcpy(puntero_al_final + 1, strv[i]);
        } 
        if (strv[i+1] != NULL) {
            puntero_al_final[0] = sep;
        }
    }
    puntero_al_final = '\0';
    return cadena;
}

void free_strv(char *strv[]) {
    for (size_t i = 0; strv[i] != NULL; i++) free(strv[i]);
    free(strv);
}

//gcc -g -O2 -std=gnu18 -Wall -pedantic -Wformat=2 -Wshadow -Wpointer-arith -Wunreachable-code -Wconversion -Wno-sign-conversion -Wbad-function-cast -DCORRECTOR -Werror strutil.c -o str