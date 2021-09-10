//gcc -g -std=c11 -Wall -Wconversion -Wtype-limits -pedantic -Werror -o pruebas *.c -lm
#include "calc_helper.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "strutil.h"
#include <math.h>

int main() {
    bool error = false;
    char* buffer = NULL;
    size_t capacidad = 0;
    //Chequeo que la entrada no sea una cadena vacia.
    while (getline(&buffer, &capacidad, stdin) != EOF) {
        //Transformo la cadena ingresada en un arreglo de cadenas y creo la pila de enteros.
        char** operacion = dc_split(buffer);
        pilanum_t* numeros = pilanum_crear();
        //Recorro el arreglo de cadenas y dependiendo del tipo de dato apilo u opero y apilo.
        struct calc_token token;
        calc_num res;
        calc_num tope_final;
        for (int i = 0; operacion[i] != NULL; i++) {
            //Si la cadena no es admitida imprime un error, en caso contrario sigue la ejecución y la funcion le da un tipo al token.
            if (!calc_parse(operacion[i], &token)) {
                error = true;
                break;
            }
            //Pasos a seguir si el token es un número.
            if (token.type == TOK_NUM) {
                apilar_num(numeros, token.value);
            }
            //Pasos a seguir si el token es un operador.
            if (token.type == TOK_OPER) {
                calc_num n1;
                calc_num n2;
                calc_num n3;
                //Condición de suma.
                if (token.oper.op == OP_ADD) {
                    if (!desapilar_num(numeros, &n2) || !desapilar_num(numeros, &n1)){
                        error = true;
                        break;
                    } 
                    apilar_num(numeros, n1 + n2);
                }
                //Condicion de resta.
                if (token.oper.op == OP_SUB) {
                    if (!desapilar_num(numeros, &n2) || !desapilar_num(numeros, &n1)) {
                        error = true;
                        break;
                    }
                    apilar_num(numeros, n1 - n2);
                }
                //Condicion de multiplicación.
                if (token.oper.op == OP_MUL) {
                    if (!desapilar_num(numeros, &n2) || !desapilar_num(numeros, &n1)) {
                        error = true;
                        break;
                    }
                    apilar_num(numeros, n1 * n2);
                }
                //Condicion de división entera.
                if (token.oper.op == OP_DIV) {
                    if (!desapilar_num(numeros, &n2) || !desapilar_num(numeros, &n1) || n2 == 0) {
                        error = true;
                        break;
                    }
                    apilar_num(numeros, n1 / n2);
                }
                //Condicion de potencia.
                if (token.oper.op == OP_POW) {
                    if (!desapilar_num(numeros, &n2) || !desapilar_num(numeros, &n1) || n2 < 0) {
                        error = true;
                        break;
                    }
                    double potencia = pow((double)n1, (double)n2);
                    apilar_num(numeros, (calc_num)potencia);
                }
                //Condicion de logaritmo.
                if (token.oper.op == OP_LOG) {
                    if (!desapilar_num(numeros, &n2) || !desapilar_num(numeros, &n1) || n2 < 2) {
                        error = true;
                        break;
                    }
                    res = (calc_num) (log10((double) n1) / log10((double) n2));
                    apilar_num(numeros, res);
                }
                //Condicion de raíz cuadrada.
                if (token.oper.op == OP_RAIZ) {
                    if (!desapilar_num(numeros, &n2) || n2 < 0){
                        error = true;
                        break;
                    } 
                    double cuadrado = sqrt((double)n2);
                    apilar_num(numeros, (calc_num)cuadrado);
                }
                //Condicion de operador ternario.
                if (token.oper.op == OP_TERN) {
                    if (!desapilar_num(numeros, &n3) || !desapilar_num(numeros, &n2) || !desapilar_num(numeros, &n1)) {
                        error = true;
                        break;
                    }
                    apilar_num(numeros, n1 ? n2 : n3);
                }
            }
        }
        desapilar_num(numeros, &tope_final);
        if (!pila_esta_vacia(numeros)) error = true;
        if (error) {
            fprintf(stdout, "%s\n", "ERROR");
            error = false;
            pilanum_destruir(numeros);
            free_strv(operacion);
            continue;
        } 
        fprintf(stdout, "%ld\n", tope_final);
        pilanum_destruir(numeros);
        free_strv(operacion);
    }
    //Libero toda la memoria utilizada.
    free(buffer);
    return 0;
}




//gcc -g -O2 -std=gnu18 -Wall -pedantic -Wformat=2 -Wshadow -Wpointer-arith -Wunreachable-code -Wconversion -Wno-sign-conversion -Wbad-function-cast -DCORRECTOR -Werror dc.c calc_helper.c pila.c strutil.c -o dc -lm 