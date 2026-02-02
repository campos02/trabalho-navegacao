#define DIJKSTRA_H
#define DIJKSTRA_H

#include "vertices.h"
#include <stdint.h>

typedef struct Distancia
{
    uint64_t predecessor;
    double distancia;
} Distancia;

typedef struct Caminho
{
    int tamanho;
    uint64_t *caminho;
} Caminho;

typedef struct
{
    uint64_t vertice; // Identificador do vértice
    double distancia; // Distância acumulada até ele
} NoHeap;

typedef struct
{
    NoHeap *dados;  // Vetor de nós
    int tamanho;    // Quantidade atual de elementos
    int capacidade; // Capacidade máxima do heap
} HeapMinimo;

//funçoes para heap/ fila de prioridade

HeapMinimo *criarHeap(int capacidade);
void inserirHeap(HeapMinimo *heap, int vertice, double distancia);
NoHeap removerMinimo(HeapMinimo *heap);
int heapVazio(HeapMinimo *heap);

Distancia *calcular_distancias_dijkstra(Vertices *vertices, uint64_t origem);
Caminho construir_caminho_dijkstra(Vertices *vertices,
                                   uint64_t origem,
                                   uint64_t destino,
                                   Distancia *distancias);
