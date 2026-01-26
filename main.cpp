#include "ler_dados.h"
#include "dijkstra.h"
#include "trie.h"

#include <stdlib.h>

int main(int argc, char *argv[])
{
    QFile file;
    Vertices vertices = ler_vertices(&file);
    free(vertices.vertices);


    double* distancias = dijkstra(&vertices, origem, predecessores);

    if (distancias == NULL) {
        printf("Erro ao calcular Dijkstra.\n");
        return 1;
    }

    int tamanhoCaminho;
    uint64_t* caminho = reconstruirCaminho(origem, destino, predecessores, &vertices, &tamanhoCaminho);

    if (caminho != NULL) {
        printf("Caminho encontrado com %d vértices:\n", tamanhoCaminho);
        for (int i = 0; i < tamanhoCaminho; i++) {
            printf("%lu", caminho[i]);
            if (i < tamanhoCaminho - 1) printf(" -> ");
        }
        printf("\n");
        free(caminho);
         } else {
        printf("Não há caminho.\n");
    }

    // Liberar memória
    free(distancias);

    liberarListasArestas(&vertices);
    free(vertices.vertices);

    return 0;

}
