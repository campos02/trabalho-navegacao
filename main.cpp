#include "dijkstra.h"
#include "ler_dados.h"
#include "trie.h"

#include <stdlib.h>

int main(int argc, char *argv[])
{
    QFile file;

    Vertices vertices = ler_vertices(&file);
    ler_arestas(&vertices, &file);

    ArvoreTrie *trie = criar_trie();
    carregar_nodes_to_label(trie, &file);

    //-----TESTE DA TRIE.
    int qtd = 0;
    char **res = buscar_prefixo_trie(trie, "marques", &qtd);

    printf("Resultados (%d):\n", qtd);
    for (int i = 0; i < qtd; i++) {
        printf("  %s\n", res[i]);
        free(res[i]);
    }
    free(res);

    printf("\n");

    //------

    Distancia *distancias = calcular_distancias_dijkstra(&vertices, vertices.vertices[0].id);
    Caminho caminho = construir_caminho_dijkstra(&vertices,
                                                 vertices.vertices[0].id,
                                                 vertices.vertices[800].id,
                                                 distancias);

    for (int i = vertices.quantidade - caminho.tamanho; i < vertices.quantidade - 1; i++) {
        Aresta aresta = aresta_rua(&vertices, caminho.caminho[i], caminho.caminho[i + 1]);
        if (aresta.destino != 0 && aresta.nome[0] != '\0') {
            if (!strcmp(aresta.tipo, "primary") || !strcmp(aresta.tipo, "secondary"))
                printf("Av. %s -> ", aresta.nome);
            else
                printf("R. %s -> ", aresta.nome);
        }
    }

    printf("Destino\n");

    free(distancias);
    free(caminho.caminho);
    liberar_trie(trie);
    liberar_vertices(&vertices);

    return 0;
}
