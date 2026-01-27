#ifndef ARESTA_H
#define ARESTA_H

// Manter o tamanho máximo do id consistente mesmo em plataformas diferentes
#include <stdint.h>

typedef struct Aresta {
    uint64_t osmid;
    uint64_t destino;
    double tamanho;
} Aresta;

#endif // ARESTA_H
