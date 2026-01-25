#ifndef VERTICES_H
#define VERTICES_H

#include "vertice.h"

typedef struct Vertices
{
    int quantidade;
    Vertice* vertices;
} Vertices;

int indice_por_id(Vertices* vertices, uint64_t id);

#endif // VERTICES_H
