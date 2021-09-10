#include "box.h"

struct box {
    size_t frecuencia;
    const char* cadena;
};

int boxcmp(const void* a, const void* b) {
    //Diferentes frecuencias.
    if (box_frecuencia((box_t*) (a)) > box_frecuencia((box_t*) (b))) return 1;
    if (box_frecuencia((box_t*) (a)) < box_frecuencia((box_t*) (b))) return -1;

    //Mismas frecuencias.
    return (-1) * strcmp( box_cadena((box_t*) (a)), box_cadena((box_t*) (b)));
}

box_t* box_crear(size_t frecuencia, const char* cadena) {
    box_t* box = malloc(sizeof(box_t));
    if (!box) return NULL;

    box->frecuencia = frecuencia;
    box->cadena = cadena;
    return box;
}

size_t box_frecuencia(box_t* box) {
    return box->frecuencia;
}

const char* box_cadena(box_t* box) {
    return box->cadena;
}

void box_imprimir(box_t* box) {
    printf("%ld %s\n", box->frecuencia, box->cadena);
}

void box_destruir(box_t* box) {
    free(box);
}
