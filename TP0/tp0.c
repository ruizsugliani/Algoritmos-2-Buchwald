#include "tp0.h"

/* *****************************************************************
 *                     FUNCIONES A COMPLETAR                       *
 *         (ver en tp0.h la documentación de cada función)         *
 * *****************************************************************/

void swap(int *x, int *y) {
    int temp = *x; 
    *x = *y;
    *y = temp;
}


int maximo(int vector[], int n) {      
   if (n == 0) {
       return -1;
   }
    int max_pos = 0;
    for (int i = 1; i < n; i++) {
        if (vector[i] > vector[max_pos]) {
            max_pos = i;
        }
    }
    return max_pos;
}



int comparar(int vector1[], int n1, int vector2[], int n2) {
    for (int i = 0; i < n1 || i < n2; i++) {
        if (i == n1 && n1 < n2) {
            return -1;
        } if (i == n2 && n2 < n1) {
            return 1;
        }
        if (vector1[i] < vector2[i]) {
            return -1;
        } if (vector2 [i] < vector1[i]) {
            return 1;
        }      
    }  
    return 0;
}


void seleccion(int *numeros, int n) {
    if (n == 1 || n == 0) {
        return;
    }
    int j = 0;
    for (int i = n-1; i > 0; i--) {
        int max = maximo(numeros, n-j);
        swap(&numeros[i], &numeros[max]);
        j++;
    }
}
