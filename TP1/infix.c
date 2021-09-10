#include "cola.h"
#include "pila.h"
#include "calc_helper.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "strutil.h"
#include <math.h>

int main() {
    char* buffer = NULL;
    size_t capacidad = 0;
    while (getline(&buffer, &capacidad, stdin) != EOF) {
        char** entrada = infix_split(buffer);
        //Creo la pila de operadores y la cola de salida.
        pila_t* opers = pila_crear();
        cola_t* salida = cola_crear();
        //Recorro el arreglo de cadenas y dependiendo del tipo de dato apilo o encolo.
        struct calc_token token;

        for (int i = 0; entrada[i] != NULL; i++) {
            calc_parse(entrada[i], &token);
            //Pasos a seguir si el token es un número (simplemente encolarlo).
            if (token.type == TOK_NUM) {
                if (cola_encolar(salida, entrada[i]));
            
            }
            else if (token.type == TOK_LPAREN) {
                    pila_apilar(opers, entrada[i]);
                    continue;
            }
            //Condición de paréntesis derecho (desapilar y encolar hasta encontrar un paréntesis izquierdo).
            else if (token.type == TOK_RPAREN) {
                struct calc_token tope_p;
                while (true) {
                    const char * tope = (char*) pila_ver_tope(opers);
                    calc_parse(tope, &tope_p);
                    if (tope_p.type == TOK_LPAREN) {
                        pila_desapilar(opers);
                        break;
                    }
                    else cola_encolar(salida, pila_desapilar(opers));
                }
            } 
            //Pasos a seguir si el token es un operador.
            else if (token.type == TOK_OPER) {   
                if (pila_esta_vacia(opers)) {
                    pila_apilar(opers, entrada[i]);        
                    continue;
                }
                while(true) {
                    struct calc_token tope_p;
                    const char * tope = (char*) pila_ver_tope(opers);
                    calc_parse(tope, &tope_p);
                    
                    if ((token.oper.asociatividad == IZQ && token.oper.precedencia <= tope_p.oper.precedencia) || (token.oper.asociatividad == DER && token.oper.precedencia < tope_p.oper.precedencia)) {
                        cola_encolar(salida, pila_desapilar(opers));
                        if (!pila_esta_vacia(opers)) continue;
                    } break;
                }
                pila_apilar(opers, entrada[i]);                   
            }
        }
        //No hay más tokens para leer, si todavía hay operadores en la pila, se encolan.
        while (!pila_esta_vacia(opers)) {
            cola_encolar(salida, pila_desapilar(opers));
        }
        //Imprimo la entrada en notación posfija.
        while (!cola_esta_vacia(salida)) {
            fprintf(stdout, "%s ", (char*) cola_desencolar(salida));
        }
        fprintf(stdout, "%s","\n");
        pila_destruir(opers);
        cola_destruir(salida, NULL);
        free_strv(entrada);
    }
    //Libero toda la memoria utilizada.
    free(buffer);
    return 0;
}



//gcc -g -O2 -std=gnu18 -Wall -pedantic -Wformat=2 -Wshadow -Wpointer-arith -Wunreachable-code -Wconversion -Wno-sign-conversion -Wbad-function-cast -DCORRECTOR -Werror infix.c cola.c calc_helper.c pila.c strutil.c -o infix