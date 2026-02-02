#ifndef ARESTA_H
#define ARESTA_H

// Manter o tamanho máximo do id consistente mesmo em plataformas diferentes
#include <stdint.h>

typedef struct Aresta
{
    uint64_t osmid;
    uint64_t destino;
    double tamanho;
    int unidirecional;          // 1 = mão única (só vai de origem para destino)
                                // 0 = mão dupla (vai e volta)
} Aresta;

#endif // ARESTA_H
