#define DIJKSTRA_H
#define DIJKSTRA_H
#include <stdlib.h>
#include <stdio.h>

#include "vertices.h"  // Usa SEUS arquivos
// no da lista de adjacencia

/*typedef struct ListaNodo {
    uint64_t destino;                // Vértice de destino da aresta
    double peso;                // Peso da aresta (distância)
        int unidirecional;    // Adicionei para mão única
    struct ListaNodo* proxima;     // Próxima aresta na lista
} ListaNodo;

typedef struct {
    int numVertices;            // Número total de vértices
    ListaNodo** listaArray;     // Vetor de listas de adjacência
} GrafoLista; */

//fila de prioridade com heap minimo

typedef struct {
    int vertice;                // Identificador do vértice
    double distancia;           // Distância acumulada até ele
} NoHeap;

typedef struct {
    NoHeap* dados;              // Vetor de nós
    int tamanho;                // Quantidade atual de elementos
    int capacidade;             // Capacidade máxima do heap
} HeapMinimo;

//funçoes para heap/ fila de prioridade

HeapMinimo* criarHeap(int capacidade);
void inserirHeap(HeapMinimo* heap, int vertice, double distancia);  
NoHeap removerMinimo(HeapMinimo* heap);
int heapVazio(HeapMinimo* heap);

double* calcular_distancias_dijkstra(Vertices* vertices, uint64_t origem);
uint64_t* reconstruir_caminho_dijkstra(Vertices* vertices, uint64_t origem, 
                                      uint64_t destino, double* distancias, 
                                      int* tamanho_caminho);
