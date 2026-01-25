#include <stdint.h>
#include "vertices.h"

int indice_por_id(Vertices* vertices, uint64_t id) {
    int indice = id % vertices->quantidade;

    // Lidar com colisões
    if (vertices->vertices[indice].id != 0)
        indice /= 2;

    while (indice < vertices->quantidade - 1 && vertices->vertices[indice].id != 0)
        indice++;

    return indice;
}
