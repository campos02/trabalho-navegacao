#ifndef VERTICE_H
#define VERTICE_H

// Manter o tamanho máximo do id consistente mesmo em plataformas diferentes
#include <stdint.h>

#include "aresta.h"

typedef struct Vertice
{
    uint64_t id;
    double x;
    double y;

    int quantidade_arestas;
    Aresta* arestas;
} Vertice;

#endif // VERTICE_H
