#include "dijkstra.h"
#include "ler_dados.h"
#include "trie.h"

#include <stdlib.h>

int main(int argc, char *argv[])
{
    QFile file;
    Vertices vertices = ler_vertices(&file);
    ler_arestas(&vertices, &file);

    Distancia *distancias = calcular_distancias_dijkstra(&vertices, vertices.vertices[0].id);
    Caminho caminho = construir_caminho_dijkstra(&vertices,
                                                 vertices.vertices[0].id,
                                                 vertices.vertices[8080].id,
                                                 distancias);

    for (int i = vertices.quantidade - caminho.tamanho; i < vertices.quantidade; i++) {
        printf("%lu ", caminho.caminho[i]);
    }

    printf("\n");

    free(distancias);
    free(caminho.caminho);
    liberar_vertices(&vertices);

    return 0;
}
