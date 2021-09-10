#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void radixsort_chars(char **arr, size_t tam);
void counting_chars(char **arr, size_t tam, int indice, int max, int min);
int len_cadenas(char** cadenas);
void imprimir_cadenas(char** cadenas, int len, int num);