#include <stdint.h>
#include <stdlib.h>

#include "vertices.h"

int indice_por_id(Vertices* vertices, uint64_t id) {
    int indice = id % vertices->quantidade;

    // Lidar com colisões
    if (vertices->vertices[indice].id != id)
        indice /= 2;

    while (indice < vertices->quantidade - 1 && vertices->vertices[indice].id != id)
        indice++;

    return indice;
}

void liberar_vertices(Vertices* vertices) {
    for (int i = 0; i < vertices->quantidade; i++)
        free(vertices->vertices[i].arestas);

    free(vertices->vertices);
}
