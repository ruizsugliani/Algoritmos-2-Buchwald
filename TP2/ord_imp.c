#define MAX_CANT_CARACTERES 14
#include "ord_imp.h"

void counting_chars(char**arr, size_t tam, int indice, int max, int min){

    char **aux = calloc(tam, sizeof(char*));
    if (!aux) return;

    int rango = (max - min);

    int *frecuencia = calloc(rango + 1, sizeof(int));
    if (!frecuencia){
        free(aux);
        return;
    }

    for (size_t i = 0; i < tam; i++){
        if (strlen(arr[i]) <= indice){
            frecuencia[0]++;
        }else{
            int pos = max - arr[i][indice];
            frecuencia[pos]++;
        }
    }

    int *suma_acumulativa = calloc(rango + 1, sizeof(int));
    if (!suma_acumulativa){
        free(aux);
        free(frecuencia);
        return;
    }

    int sum = 0;
    for (size_t j = 1; j < rango + 1; j++){
        sum += frecuencia[j - 1];
        suma_acumulativa[j] = sum;
    }

    for (size_t r = 0; r < tam; r++){
        if (strlen(arr[r]) <= indice){
            aux[suma_acumulativa[0]] = arr[r];
            suma_acumulativa[0]++;
        }else{
            int pos = max - arr[r][indice];
            aux[suma_acumulativa[pos]] = arr[r];
            suma_acumulativa[pos]++;
        }
    }

    for (size_t i = 0; i < tam ; i++) {
        arr[i] = aux[i];
    }

    free(aux);
    free(frecuencia);
    free(suma_acumulativa);
}

void radixsort_chars(char** arr, size_t tam) {
    int min = 0;
    int max = 255;

    for (int indice = MAX_CANT_CARACTERES; indice >= 0 ; indice--){
        counting_chars(arr, tam, indice, max, min);
    }   
}

int len_cadenas(char**cadenas) {
    int res = 0;
    for (int i = 0; cadenas[i]; i++) res++;
    return res;
}

void imprimir_cadenas(char **cadenas, int len, int num){
    printf("%d: ", num);

    for (int i = len - 1; i > 0; i--){
        printf("%s, ", cadenas[i]);
    }
    printf("%s\n", cadenas[0]);
}