#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>

//Definición de los struct nodo y lista.
typedef struct nodo {
    void* dato;
    struct nodo* prox;
} nodo_t;

typedef struct lista {
    nodo_t* prim;
    nodo_t* ult;
    size_t largo;
} lista_t;

typedef struct lista_iter {
    nodo_t* anterior;
    nodo_t* actual;
    lista_t* lista;
} lista_iter_t;

// Definición de nodo_crear.
nodo_t* nodo_crear(void* valor) {
    nodo_t* nodo = malloc(sizeof(nodo_t));
    if (nodo == NULL) {
        free(nodo);
        return NULL;
    }
    nodo->dato = valor;
    nodo->prox = NULL;
    return nodo;
}

/* *****************************************************************
 *                    PRIMITIVAS DE LA LISTA
 * *****************************************************************/

lista_t *lista_crear(void) {
    lista_t* lista = malloc(sizeof(lista_t));
    if (!lista) return NULL;
    lista->prim = NULL;
    lista->ult = NULL;
    lista->largo = 0;
    return lista;
}

bool lista_esta_vacia(const lista_t *lista) {
    return lista->largo == 0;
}

size_t lista_largo(const lista_t *lista) {
    return lista->largo;
}

void *lista_ver_primero(const lista_t *lista) {
    if (lista_esta_vacia(lista)) return NULL;
    return lista->prim->dato;
}

void *lista_ver_ultimo(const lista_t* lista) {
    if (lista_esta_vacia(lista)) return NULL;
    return lista->ult->dato;
}

bool lista_insertar_primero(lista_t *lista, void *dato) {
    nodo_t* nuevo_nodo = nodo_crear(dato);
    if (!nuevo_nodo) return false;
    
    if (lista_esta_vacia(lista)) {
        lista->prim = nuevo_nodo;
        lista->ult = nuevo_nodo;
    } else {
    nuevo_nodo->prox = lista->prim;
    lista->prim = nuevo_nodo;
    }
    lista->largo++;
    return true;
}

bool lista_insertar_ultimo(lista_t *lista, void *dato) {
    nodo_t* nuevo_nodo = nodo_crear(dato);
    if (!nuevo_nodo) return false;

    if (lista_esta_vacia(lista)) {
        lista->prim = nuevo_nodo;
        lista->ult = nuevo_nodo;
    } else {
    lista->ult->prox = nuevo_nodo;
    lista->ult = nuevo_nodo;
    lista->ult->prox = NULL;
    }
    lista->largo++;
    return true;
}

void *lista_borrar_primero(lista_t *lista) {
    if (lista_esta_vacia(lista)) return NULL;
    void* dato = lista->prim->dato;
    nodo_t* nodo_ant = lista->prim;
    lista->prim = nodo_ant->prox;
    free(nodo_ant);
    lista->largo--;
    return dato;
}

void lista_destruir(lista_t *lista, void (*destruir_dato)(void *)) {
    while (!lista_esta_vacia(lista)) {
        void* dato = lista_borrar_primero(lista);
        if (destruir_dato) destruir_dato(dato); 
    }
    free(lista);
}

/* *****************************************************************
 *                 PRIMITIVA DEL ITERADOR INTERNO
 * *****************************************************************/

void lista_iterar(lista_t *lista, bool visitar(void *dato, void *extra), void *extra) {
    nodo_t* actual = lista->prim;
    while(actual) {
        void* dato = actual->dato;
        if (visitar(dato, extra)) {
            actual = actual->prox;
            continue;
        }
        break;
    }
}

/* *****************************************************************
 *                 PRIMITIVAS DEL ITERADOR EXTERNO
 * *****************************************************************/

lista_iter_t *lista_iter_crear(lista_t *lista) {
    lista_iter_t* iter = malloc(sizeof(lista_iter_t));
    if (!iter) return NULL;
    iter->anterior = NULL;
    iter->actual = lista->prim;
    iter->lista = lista;
    return iter;
}

bool lista_iter_avanzar(lista_iter_t *iter) {
    if (!iter->actual) return false;
    iter->anterior = iter->actual;
    iter->actual = iter->actual->prox;
    return true;
}

bool lista_iter_al_final(const lista_iter_t *iter) {
    return iter->actual == NULL;
}

void *lista_iter_ver_actual(const lista_iter_t *iter) {
    if (lista_iter_al_final(iter)) return NULL;
    return iter->actual->dato;
}

void lista_iter_destruir(lista_iter_t *iter) {
    free(iter);
}

bool lista_iter_insertar(lista_iter_t *iter, void *dato) {
    nodo_t* nuevo_nodo = nodo_crear(dato);
    if (!nuevo_nodo) return false;
    if (lista_esta_vacia(iter->lista)) {
        iter->lista->prim = nuevo_nodo;
        iter->lista->ult = nuevo_nodo;
    }
    if (!iter->anterior) {
        iter->lista->prim = nuevo_nodo;
        nuevo_nodo->prox = iter->actual;
        iter->actual = nuevo_nodo;
    } else {
        iter->anterior->prox = nuevo_nodo;
        nuevo_nodo->prox = iter->actual;
        iter->actual = nuevo_nodo;
        if (!iter->actual->prox) iter->lista->ult = iter->actual;
    }
    iter->lista->largo ++;
    return true;
}

void *lista_iter_borrar(lista_iter_t *iter) {
    if (lista_iter_al_final(iter)) return NULL;
    nodo_t* nodo_a_borrar = iter->actual;
    void* dato = iter->actual->dato;
    iter->actual = iter->actual->prox;
    if (iter->anterior) iter->anterior->prox = iter->actual;
    if (!iter->anterior) iter->lista->prim = iter->actual;
    if (lista_iter_al_final(iter)) iter->lista->ult = iter->anterior;
    free(nodo_a_borrar);
    iter->lista->largo--;
    return dato;
}

