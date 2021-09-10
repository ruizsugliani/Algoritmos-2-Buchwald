#include "vector.h"

#define VOLUMEN_INICIAL 10

/*******************************************************************
 *                          ESTRUCTURA
 ******************************************************************/

struct vector {
    size_t tam;
    int *datos;
    int pos;
};

/*******************************************************************
 *                          AUXILIARES
 ******************************************************************/
int maximo(int *arr, int tam, int *minimo){
    int max = arr[0];
    int min = arr[0];

    for (int i = 1; i < tam; i++) {
        if (arr[i] > max) max = arr[i];

        if (arr[i] < min) min = arr[i];
    }
    *minimo = min;
    return max;
}


void counting(int *arr, int tam, int max, int min){
    int *aux = calloc(tam, sizeof(int));
    if (!aux) return;

    int rango = (max - min);

    int *frecuencia = calloc(rango + 1, sizeof(int));
    if (!frecuencia){
        free(aux);
        return;
    }

    for (size_t i = 0; i < tam; i++){
        int pos = max - arr[i];
        frecuencia[pos]++;
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
        int pos = max - arr[r];
        aux[suma_acumulativa[pos]] = arr[r];
        suma_acumulativa[pos]++;
    }

    for (size_t i = 0; i < tam ; i++) {
        arr[i] = aux[i];
    }

    free(aux);
    free(frecuencia);
    free(suma_acumulativa);
}


/*******************************************************************
 *                          PRIMITIVAS
 ******************************************************************/
void vector_destruir(vector_t *vector){
    free(vector->datos);
    free(vector);
}

bool vector_guardar(vector_t *vector, int valor){
    if (vector->pos >= vector->tam){
        if (!vector_redimensionar(vector, 2 * vector->tam)){
            return false;
        }
    }
    vector->datos[vector->pos] = valor;
    vector->pos++;
    return true;
}

int vector_obtener(vector_t *vector, size_t pos){
    return vector->datos[pos];
}

int vector_largo(vector_t *vector){
    return vector->pos;
}

vector_t *vector_crear(){
    vector_t *vector = malloc(sizeof(vector_t));

    if (vector == NULL){
        return NULL;
    }
    vector->datos = malloc(VOLUMEN_INICIAL * sizeof(int));

    if (VOLUMEN_INICIAL > 0 && vector->datos == NULL){
        free(vector);
        return NULL;
    }
    vector->tam = VOLUMEN_INICIAL;
    vector->pos = 0;
    return vector;
}

bool vector_redimensionar(vector_t *vector, size_t tam_nuevo){
    int *datos_nuevo = realloc(vector->datos, tam_nuevo * sizeof(int));

    // Cuando tam_nuevo es 0, es correcto si se devuelve NULL.
    // En toda otra situación significa que falló el realloc.
    if (tam_nuevo > 0 && datos_nuevo == NULL){
        return false;
    }

    vector->datos = datos_nuevo;
    vector->tam = tam_nuevo;
    return true;
}

void vector_ordenar(vector_t *vector){
    size_t largo = vector_largo(vector);
    int min;
    int max = maximo(vector->datos, (int)largo, &min);
    counting(vector->datos, (int)largo, max, min);
}

void vector_imprimir(vector_t *vector){
    size_t largo = vector_largo(vector);
    for (int i = 0; i < (int)largo; i++){
        printf("NUMERO %d\n", vector->datos[i]);
    }
}