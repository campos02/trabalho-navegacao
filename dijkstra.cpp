#include "dijkstra.h"

#include <limits.h>
#include <float.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// fila de pprioridade/ heap minimo

// Cria um heap mínimo com capacidade inicial
HeapMinimo *criar_heap(int capacidade)
{
    if (capacidade <= 0)
        capacidade = 10;

    HeapMinimo *heap = (HeapMinimo *) malloc(sizeof(HeapMinimo));
    heap->dados = (NoHeap *) malloc(capacidade * sizeof(NoHeap));
    heap->tamanho = 0;
    heap->capacidade = capacidade;

    return heap;
}

// Insere um elemento no heap mantendo a propriedade de heap mínimo
void inserir_heap(HeapMinimo *heap, uint64_t vertice, double distancia)
{
    // Aumenta capacidade se necessário
    if (heap->tamanho >= heap->capacidade) {
        heap->capacidade *= 2;
        heap->dados = (NoHeap *) realloc(heap->dados, heap->capacidade * sizeof(NoHeap));
    }

    // Insere no final
    heap->dados[heap->tamanho].vertice = vertice;
    heap->dados[heap->tamanho].distancia = distancia;

    // Sobe no heap (ajuste para cima)
    int i = heap->tamanho;
    heap->tamanho++;

    while (i > 0 && heap->dados[(i - 1) / 2].distancia > heap->dados[i].distancia) {
        // Troca com o pai
        NoHeap temp = heap->dados[i];
        heap->dados[i] = heap->dados[(i - 1) / 2];
        heap->dados[(i - 1) / 2] = temp;

        i = (i - 1) / 2;
    }
}

// Remove e retorna o elemento com menor distância
NoHeap extrair_minimo(HeapMinimo *heap)
{
    if (heap->tamanho == 0) {
        NoHeap vazio = {0, -1.0};
        return vazio;
    }

    // O menor está na raiz (índice 0)
    NoHeap minimo = heap->dados[0];

    // Move último elemento para a raiz
    heap->dados[0] = heap->dados[heap->tamanho - 1];
    heap->tamanho--;

    // Ajusta para baixo (heapify) =  nao sei se é necessario
    int i = 0;
    while (1) {
        int menor = i;
        int esquerda = 2 * i + 1;
        int direita = 2 * i + 2;

        if (esquerda < heap->tamanho
            && heap->dados[esquerda].distancia < heap->dados[menor].distancia) {
            menor = esquerda;
        }

        if (direita < heap->tamanho
            && heap->dados[direita].distancia < heap->dados[menor].distancia) {
            menor = direita;
        }

        if (menor != i) {
            // Troca com o menor filho
            NoHeap temp = heap->dados[i];
            heap->dados[i] = heap->dados[menor];
            heap->dados[menor] = temp;

            i = menor;
        } else {
            break;
        }
    }

    return minimo;
}

// Verifica se o heap está vazio
int heap_vazio(HeapMinimo *heap)
{
    return heap->tamanho == 0;
}

// Libera memória do heap
void liberar_heap(HeapMinimo *heap)
{
    free(heap->dados);
    free(heap);
}

// ALGORITMO DE DIJKSTRA

// Calcula distâncias mínimas a partir de um vértice origem
// RETORNA: array de distâncias ou NULL em caso de erro
Distancia *calcular_distancias_dijkstra(Vertices *vertices, uint64_t origem)
{
    if (!vertices)
        return NULL;

    int n = vertices->quantidade;
    Distancia *distancias = (Distancia *) malloc(n * sizeof(Distancia));
    if (!distancias)
        return NULL;

    // Distância "infinita" padrão
    for (int i = 0; i < n; i++) {
        distancias[i].distancia = INT_MAX / 2;
        distancias[i].predecessor = 0;
    }

    int *processado = (int *) calloc(n, sizeof(int));
    if (!processado) {
        free(distancias);
        return NULL;
    }

    int indice_origem = indice_por_id(vertices, origem);
    if (indice_origem == -1) {
        free(distancias);
        free(processado);
        return NULL;
    }

    // Distância da origem para si mesma é 0
    distancias[indice_origem].distancia = 0.0;
    distancias[indice_origem].predecessor = 0;

    HeapMinimo *heap = criar_heap(n);
    if (!heap) {
        free(distancias);
        free(processado);
        return NULL;
    }

    inserir_heap(heap, origem, 0.0);
    while (!heap_vazio(heap)) {
        // Extrai o vértice com menor distância
        NoHeap atual_heap = extrair_minimo(heap);
        uint64_t vertice_atual_id = atual_heap.vertice;

        int indice_atual = indice_por_id(vertices, vertice_atual_id);
        if (indice_atual == -1 || processado[indice_atual]) {
            continue;
        }

        processado[indice_atual] = 1;

        // Percorre todas as arestas saindo do vértice atual
        for (int i = 0; i < vertices->vertices[indice_atual].quantidade_arestas; i++) {
            // Encontra índice do vértice destino
            int indice_destino = indice_por_id(vertices,
                                               vertices->vertices[indice_atual].arestas[i].destino);
            if (indice_destino != -1 && !processado[indice_destino]) {
                // Calcula nova distância
                double nova_distancia = distancias[indice_atual].distancia
                                        + vertices->vertices[indice_atual].arestas[i].tamanho;

                // Se encontrou um caminho mais curto, atualiza
                if (nova_distancia < distancias[indice_destino].distancia) {
                    distancias[indice_destino].distancia = nova_distancia;
                    distancias[indice_destino].predecessor = vertice_atual_id;

                    // Insere no heap
                    inserir_heap(heap,
                                 vertices->vertices[indice_atual].arestas[i].destino,
                                 nova_distancia);
                }
            }
        }
    }

    // Libera memória auxiliar
    liberar_heap(heap);
    free(processado);

    return distancias;
}

// Constrói o caminho mínimo da origem ao destino
Caminho construir_caminho_dijkstra(Vertices *vertices,
                                   uint64_t origem,
                                   uint64_t destino,
                                   Distancia *distancias)
{
    Caminho caminho;
    caminho.tamanho = 0;
    caminho.caminho = NULL;

    if (!vertices || !distancias)
        return caminho;

    int indice_destino = indice_por_id(vertices, destino);
    int indice_origem = indice_por_id(vertices, origem);

    if (indice_destino == -1 || indice_origem == -1)
        return caminho;

    caminho.caminho = (uint64_t *) calloc(vertices->quantidade, sizeof(uint64_t));
    Distancia distancia_atual = distancias[indice_destino];
    int i = vertices->quantidade - 1;
    caminho.caminho[i] = destino;

    while (distancia_atual.predecessor != 0 && i > 0) {
        caminho.tamanho++;
        caminho.caminho[i] = distancia_atual.predecessor;

        int indice = indice_por_id(vertices, distancia_atual.predecessor);
        distancia_atual = distancias[indice];
        i--;
    }

    return caminho;
}

// Exemplo de como adicionar aresta considerando mão única
void adicionar_aresta(Vertices* vertices, uint64_t origem, uint64_t destino, 
                     double peso, int unidirecional) {
    
    // 1. Adiciona aresta da origem para o destino (sempre)
    adicionar_aresta_ao_vertice(vertices, origem, destino, peso, unidirecional);
    
    // 2. Se NÃO for unidirecional (mão dupla), adiciona a volta
    if (!unidirecional) {
        adicionar_aresta_ao_vertice(vertices, destino, origem, peso, 0); // 0 = não é unidirecional
    }
    // Se for unidirecional, NÃO adiciona a volta
}
