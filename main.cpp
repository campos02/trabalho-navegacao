#include "dijkstra.h"
#include "ler_dados.h"
#include "trie.h"

#include <stdio.h>
#include <stdlib.h>

#ifdef _WIN32
#include <conio.h>
#define getch() _getch()
#else
#include <ncurses.h>
#endif

int main(int argc, char *argv[])
{
    QFile file;

    Vertices vertices = ler_vertices(&file);
    ler_arestas(&vertices, &file);

    ArvoreTrie *trie = criar_trie();
    carregar_nodes_to_label(trie, &file);

    printf("Origem: ");

    Resultado *resultados_origem = NULL;
    int quantidade_resultados = 0;

    char pesquisa[TAMANHO_MAX_NOME + 1];
    pesquisa[0] = '\0';
    char c = getch();

    int i = 0;
    while (c != EOF && c != '\r' && c != '\n' && i < TAMANHO_MAX_NOME - 1) {
        if (c == '\b') {
            if (i > 0) {
                i--;
                pesquisa[i] = '\0';
            }
        } else {
            pesquisa[i] = c;
            i++;
            pesquisa[i] = '\0';
        }

        // Limpar terminal
        printf("\e[1;1H\e[2J");

        if (pesquisa[0] != '\0') {
            quantidade_resultados = 0;
            resultados_origem = buscar_prefixo_trie(trie, pesquisa, &quantidade_resultados);

            for (int j = 0; j < quantidade_resultados; j++)
                printf("%d. %s\n", j + 1, resultados_origem[j].nome);

            if (quantidade_resultados > 0)
                printf("\n");
        }

        printf("Origem: %s", pesquisa);
        c = getch();
    }

    // Limpar terminal
    printf("\e[1;1H\e[2J");
    for (int i = 0; i < quantidade_resultados; i++)
        printf("%d. %s\n", i + 1, resultados_origem[i].nome);

    int opcao_resultado_origem = -1;
    printf("\nEscolha um resultado: ");
    scanf("%d", &opcao_resultado_origem);

    opcao_resultado_origem--;
    if (opcao_resultado_origem == -1 || opcao_resultado_origem >= quantidade_resultados) {
        printf("Opcao invalida");
        return 1;
    }

    // Limpar terminal
    printf("\e[1;1H\e[2J");
    printf("Destino: ");

    Resultado *resultados_destino = NULL;
    quantidade_resultados = 0;

    pesquisa[0] = '\0';
    c = getch();

    i = 0;
    while (c != EOF && c != '\r' && c != '\n' && i < TAMANHO_MAX_NOME - 1) {
        if (c == '\b') {
            if (i > 0) {
                i--;
                pesquisa[i] = '\0';
            }
        } else {
            pesquisa[i] = c;
            i++;
            pesquisa[i] = '\0';
        }

        // Limpar terminal
        printf("\e[1;1H\e[2J");

        if (pesquisa[0] != '\0') {
            quantidade_resultados = 0;
            resultados_destino = buscar_prefixo_trie(trie, pesquisa, &quantidade_resultados);

            for (int j = 0; j < quantidade_resultados; j++)
                printf("%d. %s\n", j + 1, resultados_destino[j].nome);

            if (quantidade_resultados > 0)
                printf("\n");
        }

        printf("Destino: %s", pesquisa);
        c = getch();
    }

    // Limpar terminal
    printf("\e[1;1H\e[2J");
    for (int i = 0; i < quantidade_resultados; i++)
        printf("%d. %s\n", i + 1, resultados_destino[i].nome);

    int opcao_resultado_destino = -1;
    printf("\nEscolha um resultado: ");
    scanf("%d", &opcao_resultado_destino);

    opcao_resultado_destino--;
    if (opcao_resultado_destino == -1 || opcao_resultado_destino >= quantidade_resultados) {
        printf("Opcao invalida");
        return 1;
    }

    Distancia *distancias
        = calcular_distancias_dijkstra(&vertices, resultados_origem[opcao_resultado_origem].id);

    Caminho caminho = construir_caminho_dijkstra(&vertices,
                                                 resultados_origem[opcao_resultado_origem].id,
                                                 resultados_destino[opcao_resultado_destino].id,
                                                 distancias);

    for (int i = vertices.quantidade - caminho.tamanho; i < vertices.quantidade - 1; i++) {
        Aresta aresta = aresta_rua(&vertices, caminho.caminho[i], caminho.caminho[i + 1]);

        // Verificar se a aresta é válida e tem nome
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
