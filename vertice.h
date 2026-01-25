#ifndef VERTICE_H
#define VERTICE_H

// Manter o tamanho máximo do id consistente mesmo em plataformas diferentes
#include <stdint.h>

typedef struct Vertice
{
    uint64_t id;
    double x;
    double y;
} Vertice;

#endif // VERTICE_H
