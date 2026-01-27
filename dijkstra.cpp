
#include "dijkstra.h"
#include <stdlib.h>
#include <float.h>
#include <stdio.h>
#include <string.h>

// fila de pprioridade/ heap minimo

// Cria um heap mínimo com capacidade inicial
HeapMinimo* criar_heap(int capacidade) {
    if (capacidade <= 0)
        capacidade = 10;
    
    HeapMinimo* heap = (HeapMinimo*)malloc(sizeof(HeapMinimo));
    heap->dados = (NoHeap*)malloc(capacidade * sizeof(NoHeap));
    heap->tamanho = 0;
    heap->capacidade = capacidade;

    return heap;
}

// Insere um elemento no heap mantendo a propriedade de heap mínimo
void inserir_heap(HeapMinimo* heap, uint64_t vertice, double distancia) {
    // Aumenta capacidade se necessário
    if (heap->tamanho >= heap->capacidade) {
        heap->capacidade *= 2;
        heap->dados = (NoHeap*)realloc(heap->dados, heap->capacidade * sizeof(NoHeap));
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
NoHeap extrair_minimo(HeapMinimo* heap) {
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
        
        if (esquerda < heap->tamanho && 
            heap->dados[esquerda].distancia < heap->dados[menor].distancia) {
            menor = esquerda;
        }
        
        if (direita < heap->tamanho && 
            heap->dados[direita].distancia < heap->dados[menor].distancia) {
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
int heap_vazio(HeapMinimo* heap) {
    return heap->tamanho == 0;
}

// Libera memória do heap
void liberar_heap(HeapMinimo* heap) {
    free(heap->dados);
    free(heap);
}

// ALGORITMO DE DIJKSTRA

// Calcula distâncias mínimas a partir de um vértice origem
// RETORNA: array de distâncias ou NULL em caso de erro
double* calcular_distancias_dijkstra(Vertices* vertices, uint64_t origem) {
    if (!vertices)
        return NULL;
    
    int n = vertices->quantidade;
    
    // Array de distâncias (inicialmente infinito)
    double* distancias = (double*)malloc(n * sizeof(double));
    if (!distancias)
        return NULL;
    
    // Array para marcar vértices processados
    int* processado = (int*)calloc(n, sizeof(int));
    if (!processado) {
        free(distancias);
        return NULL;
    }

    // Encontra índice da origem usando SUA função
    int indice_origem = indice_por_id(vertices, origem);
    if (indice_origem == -1) {
        free(distancias);
        free(processado);
        return NULL;
    }
    
    // Distância da origem para si mesma é 0
    distancias[indice_origem] = 0.0;

    // Cria heap mínimo
    HeapMinimo* heap = criar_heap(n);
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
        // O campo `unidirecional` determina se a aresta é de mão única:
        // - Se `unidirecional = 1`: só podemos percorrer de origem para destino
        // - Se `unidirecional = 0`: podemos percorrer nos dois sentidos
        // 
        // NO ARQUIVO `edges.json` DO SEU TRABALHO:
        // - Ruas de mão única: `unidirecional = true` (ou 1 no nosso caso)
        // - Ruas de mão dupla: `unidirecional = false` (ou 0 no nosso caso)

        for (int i = 0; i < vertices->vertices[indice_atual].quantidade_arestas; i++) {
            // Encontra índice do vértice destino
            int indice_destino = indice_por_id(vertices, vertices->vertices[indice_atual].arestas[i].destino);
            if (indice_destino != -1 && !processado[indice_destino]) {
                // Calcula nova distância
                double nova_distancia = distancias[indice_atual] + vertices->vertices[indice_atual].arestas[i].tamanho;

                // Se encontrou um caminho mais curto, atualiza
                if (nova_distancia < distancias[indice_destino]) {
                    distancias[indice_destino] = nova_distancia;

                    // Insere no heap
                    inserir_heap(heap, vertices->vertices[indice_atual].arestas[i].destino, nova_distancia);
                }
            }
        }
    }
    
    // Libera memória auxiliar
    liberar_heap(heap);
    free(processado);
    
    return distancias;
}

// Encontra o predecessor de um vértice no caminho mínimo

// Aqui verificamos se há uma aresta de i para indice_atual
// Como armazenamos apenas arestas na direção permitida, se uma rua
// for de mão única (unidirecional = 1) de i para indice_atual, então
// essa aresta existe. Se for mão única na direção contrária, não existe.
static int encontrar_predecessor(Vertices* vertices, int indice_atual, double* distancias) {
    // Procura por um vértice que tenha uma aresta para o atual e que faça parte do caminho mínimo
    for (int i = 0; i < vertices->quantidade; i++) {
        if (i == indice_atual)
            continue;

        for (int i = 0; i < vertices->vertices[indice_atual].quantidade_arestas; i++) {
            if (vertices->vertices[indice_atual].arestas[i].destino == vertices->vertices[indice_atual].id) {
                // Verifica se esta aresta faz parte do caminho mínimo
                // Usamos comparação com tolerância para erros de ponto flutuante
                double diferenca = distancias[indice_atual] - (distancias[i] + vertices->vertices[indice_atual].arestas[i].tamanho);

                // Se a diferença for muito pequena (praticamente zero)
                if (diferenca > -0.0001 && diferenca < 0.0001) {
                    return i;
                }
            }
        }
    }
    
    return -1; // Nenhum predecessor encontrado
}

// Reconstrói o caminho mínimo da origem ao destino
// RETORNA: array de IDs dos vértices no caminho ou NULL se não houver caminho
uint64_t* reconstruir_caminho_dijkstra(Vertices* vertices, uint64_t origem, 
                                      uint64_t destino, double* distancias, 
                                      int* tamanho_caminho) {
    *tamanho_caminho = 0;
    if (!vertices || !distancias)
        return NULL;
    
    int indice_destino = indice_por_id(vertices, destino);
    int indice_origem = indice_por_id(vertices, origem);
    
    if (indice_destino == -1 || indice_origem == -1) {
        return NULL;
    }
}
  
 // Inverte o caminho (origem -> destino)
