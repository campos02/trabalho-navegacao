#include "dijkstra.h"
#include "ler_dados.h"
#include "trie.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // Adicionado para usar strcmp

#ifdef _WIN32
#include <conio.h>
#endif

int main(int argc, char *argv[]) {
    // Nota: Certifique-se de que QFile e Vertices estejam definidos nos seus .h
    // Se estiver usando C puro (não C++ com Qt), verifique se QFile não deveria ser FILE*
    QFile file; 

    Vertices vertices = ler_vertices(&file);
    ler_arestas(&vertices, &file);

    ArvoreTrie *trie = criar_trie();
    carregar_nodes_to_label(trie, &file);

    int opcao_menu = 0;

    do {
        // Limpar terminal
        printf("\e[1;1H\e[2J");

        printf(" MENU: \n");
        printf("1. Calcular rota\n");
        printf("0. Sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d", &opcao_menu);
        getchar(); // limpar buffer

        if (opcao_menu == 1) {
            printf("Origem: ");

            Resultado *resultados_origem = NULL;
            int quantidade_resultados = 0;

            char pesquisa[TAMANHO_MAX_NOME];
            fgets(pesquisa, TAMANHO_MAX_NOME, stdin);

            while (pesquisa[0] != '\n') {
                int i = 0;
                while (pesquisa[i] != '\0') {
                    if (pesquisa[i] == '\n')
                        pesquisa[i] = '\0';
                    i++;
                }

                printf("\e[1;1H\e[2J");

                quantidade_resultados = 0;
                resultados_origem = buscar_prefixo_trie(trie, pesquisa, &quantidade_resultados);

                printf("Resultados de origem:\n");
                for (int j = 0; j < quantidade_resultados; j++)
                    printf("%d. %s\n", j + 1, resultados_origem[j].nome);

                if (quantidade_resultados > 0)
                    printf("\n");
                else
                    printf("Nenhum resultado\n\n");

                printf("Origem (caso haja resultados pressione Enter para escolher um): ");
                fgets(pesquisa, TAMANHO_MAX_NOME, stdin);
            }

            printf("\e[1;1H\e[2J");
            printf("Resultados de origem:\n");
            for (int i = 0; i < quantidade_resultados; i++)
                printf("%d. %s\n", i + 1, resultados_origem[i].nome);

            int opcao_resultado_origem = -1;
            printf("\nEscolha um resultado: ");
            scanf("%d", &opcao_resultado_origem);
            getchar();

            opcao_resultado_origem--;
            if (opcao_resultado_origem < 0 || opcao_resultado_origem >= quantidade_resultados) {
                printf("Opcao invalida\n");
                continue; // Volta para o menu em vez de fechar o programa
            }

            printf("\e[1;1H\e[2J");
            printf("Origem selecionada: %s\n", resultados_origem[opcao_resultado_origem].nome);
            printf("Destino: ");

            Resultado *resultados_destino = NULL;
            quantidade_resultados = 0;

            fgets(pesquisa, TAMANHO_MAX_NOME, stdin);

            while (pesquisa[0] != '\n') {
                int i = 0;
                while (pesquisa[i] != '\0') {
                    if (pesquisa[i] == '\n')
                        pesquisa[i] = '\0';
                    i++;
                }

                printf("\e[1;1H\e[2J");
                quantidade_resultados = 0;
                resultados_destino = buscar_prefixo_trie(trie, pesquisa, &quantidade_resultados);

                printf("Resultados de destino:\n");
                for (int j = 0; j < quantidade_resultados; j++)
                    printf("%d. %s\n", j + 1, resultados_destino[j].nome);

                if (quantidade_resultados > 0)
                    printf("\n");
                else
                    printf("Nenhum resultado\n\n");

                printf("Origem selecionada: %s\n", resultados_origem[opcao_resultado_origem].nome);
                printf("Destino (caso haja resultados pressione Enter para escolher um): ");
                fgets(pesquisa, TAMANHO_MAX_NOME, stdin);
            }

            printf("\e[1;1H\e[2J");
            printf("Resultados de destino:\n");
            for (int i = 0; i < quantidade_resultados; i++)
                printf("%d. %s\n", i + 1, resultados_destino[i].nome);

            int opcao_resultado_destino = -1;
            printf("\nOrigem selecionada: %s\n", resultados_origem[opcao_resultado_origem].nome);
            printf("Escolha um resultado: ");
            scanf("%d", &opcao_resultado_destino);
            getchar();

            printf("\e[1;1H\e[2J");

            opcao_resultado_destino--;
            if (opcao_resultado_destino < 0 || opcao_resultado_destino >= quantidade_resultados) {
                printf("Opcao invalida\n");
                continue;
            }

            Distancia *distancias = calcular_distancias_dijkstra(&vertices, resultados_origem[opcao_resultado_origem].id);
            Caminho caminho = construir_caminho_dijkstra(&vertices,
                                                         resultados_origem[opcao_resultado_origem].id,
                                                         resultados_destino[opcao_resultado_destino].id,
                                                         distancias);

            printf("Caminho de %s a %s:\n\n",
                   resultados_origem[opcao_resultado_origem].nome,
                   resultados_destino[opcao_resultado_destino].nome);

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
            printf("\nPressione Enter para voltar ao menu.");
            getchar();

        } else if (opcao_menu == 0) {
            printf("Saindo...\n");
        } else {
            printf("Opcao invalida. Pressione Enter para continuar...");
            getchar();
        }

    } while (opcao_menu != 0);

    // Liberação de memória fora do loop para evitar erros de ponteiro
    liberar_trie(trie);
    liberar_vertices(&vertices);

    return 0;
}
