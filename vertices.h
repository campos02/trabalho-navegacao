#ifndef VERTICES_H
#define VERTICES_H

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

typedef struct Vertices
{
    int quantidade;
    Vertice* vertices;
} Vertices;

int indice_por_id(Vertices* vertices, uint64_t id);
void liberar_vertices(Vertices* vertices);

#endif // VERTICES_H
