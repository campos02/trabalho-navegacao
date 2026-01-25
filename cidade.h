#ifndef CIDADE_H
#define CIDADE_H

// Manter o tamanho máximo do id consistente mesmo em plataformas diferentes
#include <stdint.h>

typedef struct Cidade
{
    uint64_t id;
    double x;
    double y;
} Cidade;

#endif // CIDADE_H
