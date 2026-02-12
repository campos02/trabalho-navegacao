#include "dijkstra.h"
#include "ler_dados.h"
#include "trie.h"

#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void limpar_buffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int ler_opcao_menu() {
    char buffer[32];
    int opcao = -1;

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        printf("\nErro: entrada indisponivel (stdin em EOF no Qt).\n");
        return 3; // força sair do menu
    }

    // Se não coube newline → limpa buffer
    if (!strchr(buffer, '\n'))
        limpar_buffer();

    buffer[strcspn(buffer, "\n")] = '\0';

    if (strlen(buffer) == 0)
        return -1;

    for (int i = 0; buffer[i]; i++) {
        if (!isdigit(buffer[i]))
            return -1;
    }

    opcao = atoi(buffer);
    return opcao;
}

void exibir_menu() {
    printf("\n========== SISTEMA DE ROTAS URBANAS ==========\n");
    printf("1 - Buscar nomes de ruas/avenidas\n");
    printf("2 - Calcular rota entre dois pontos\n");
    printf("3 - Sair\n");
    printf("==============================================\n");
    printf("Escolha uma opcao: ");
}

void buscar_nomes_trie(ArvoreTrie *trie) {
    char prefixo[100];

    printf("\n--- BUSCA DE NOMES ---\n");
    printf("Digite o prefixo para busca: ");

    if (fgets(prefixo, sizeof(prefixo), stdin) != NULL) {
        // Remover o newline
        prefixo[strcspn(prefixo, "\n")] = '\0';

        if (strlen(prefixo) == 0) {
            printf("\nPrefixo vazio. Operacao cancelada.\n");
            return;
        }

        int qtd = 0;
        char **res = buscar_prefixo_trie(trie, prefixo, &qtd);

        if (qtd == 0) {
            printf("\nNenhum resultado encontrado para '%s'.\n", prefixo);
        } else {
            printf("\nResultados encontrados (%d):\n", qtd);
            printf("------------------------\n");
            for (int i = 0; i < qtd; i++) {
                printf("%d. %s\n", i + 1, res[i]);
                free(res[i]);
            }
            printf("------------------------\n");
        }
        free(res);
    }
}

int ler_id_vertice(const char *mensagem, int max_id) {
    char buffer[20];
    int id = -1;

    printf("%s (0 a %d): ", mensagem, max_id);

    if (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        // Remover o newline
        buffer[strcspn(buffer, "\n")] = '\0';

        // Verificar se é um número
        int valido = 1;
        for (int i = 0; buffer[i] != '\0'; i++) {
            if (!isdigit(buffer[i])) {
                valido = 0;
                break;
            }
        }

        if (valido) {
            id = atoi(buffer);
            if (id < 0 || id > max_id) {
                id = -1;
            }
        }
    }

    return id;
}

void calcular_rota(Vertices *vertices) {
    int id_origem, id_destino;

    printf("\n--- CALCULAR ROTA ---\n");

    id_origem = ler_id_vertice("ID do ponto de origem", vertices->quantidade - 1);
    if (id_origem == -1) {
        printf("\nERRO: ID de origem invalido!\n");
        return;
    }

    id_destino = ler_id_vertice("ID do ponto de destino", vertices->quantidade - 1);
    if (id_destino == -1) {
        printf("\nERRO: ID de destino invalido!\n");
        return;
    }

    printf("\nCalculando rota de %d para %d...\n", id_origem, id_destino);

    Distancia *distancias_rota = calcular_distancias_dijkstra(vertices, id_origem);

    if (distancias_rota[id_destino].distancia == INFINITY) {
        printf("\nNao foi possivel encontrar uma rota entre os pontos %d e %d.\n",
               id_origem, id_destino);
        free(distancias_rota);
        return;
    }

    Caminho caminho = construir_caminho_dijkstra(vertices, id_origem, id_destino, distancias_rota);

    if (caminho.tamanho == 0) {
        printf("\nNao foi possivel construir o caminho entre os pontos.\n");
    } else {
        printf("\nROTA ENCONTRADA:\n");
        printf("Tamanho do caminho: %d vertices\n", caminho.tamanho);
        printf("Distancia total: %.2f\n", distancias_rota[id_destino].distancia);
        printf("----------------------------------------\n");

        int contador = 0;
        for (int i = 0; i < caminho.tamanho - 1; i++) {
            Aresta aresta = aresta_rua(vertices, caminho.caminho[i], caminho.caminho[i + 1]);

            if (aresta.destino != 0 && aresta.nome[0] != '\0') {
                if (!strcmp(aresta.tipo, "primary") || !strcmp(aresta.tipo, "secondary"))
                    printf("Av. %s", aresta.nome);
                else
                    printf("R. %s", aresta.nome);

                printf(" -> ");
                contador++;

                // Quebrar linha a cada 3 ruas para melhor visualização
                if (contador % 3 == 0)
                    printf("\n");
            }
        }
        printf("Destino\n");
        free(caminho.caminho);
    }

    free(distancias_rota);
}

int main(int argc, char *argv[])
{
    QFile file;
    int opcao;

    printf("Carregando dados do sistema...\n");

    Vertices vertices = ler_vertices(&file);
    ler_arestas(&vertices, &file);

    ArvoreTrie *trie = criar_trie();
    carregar_nodes_to_label(trie, &file);

    printf("Sistema carregado com sucesso!\n");
    printf("Total de vertices: %d\n", vertices.quantidade);

    do {
        exibir_menu();
        opcao = ler_opcao_menu();

        if (opcao == -1) {
            printf("\nOpcao invalida! Digite apenas numeros (1, 2 ou 3).\n");
            continue;
        }

        switch(opcao) {
            case 1:
                buscar_nomes_trie(trie);
                break;

            case 2:
                calcular_rota(&vertices);
                break;

            case 3:
                printf("\nEncerrando sistema...\n");
                break;

            default:
                printf("\nOpcao invalida! Digite 1, 2 ou 3.\n");
        }

    } while(opcao != 3);

    // Liberar memória
    liberar_trie(trie);
    liberar_vertices(&vertices);

    return 0;
}
