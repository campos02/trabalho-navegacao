#ifndef ARESTA_H
#define ARESTA_H

// Manter o tamanho máximo do id consistente mesmo em plataformas diferentes
#include "trie.h"
#include <stdint.h>

#define TAMANHO_MAX_TIPO 20

typedef struct Aresta
{
    uint64_t osmid;
    uint64_t destino;
    double tamanho;
    char nome[TAMANHO_MAX_NOME];
    char tipo[TAMANHO_MAX_TIPO];
} Aresta;

#endif // ARESTA_H
