#define DIJKSTRA_H
#define DIJKSTRA_H
#include <stdlib.h>
#include <stdio.h>

// no da lista de adjacencia

typedef struct ListaNodo {
    int destino;                // Vértice de destino da aresta
    double peso;                // Peso da aresta (distância)
    struct ListaNodo* proxima;     // Próxima aresta na lista
} ListaNodo;

tydef struct {
    int numVertices;            // Número total de vértices
    ListaNodo** listaArray;     // Vetor de listas de adjacência
} GrafoLista;

//fila de prioridade com heap minimo

typedef struct {
    int vertice;                // Identificador do vértice
    double distancia;           // Distância acumulada até ele
} NoHeap;

typedef struct {
    NoHeap* dados;              // Vetor de nós
    int tamanho;                // Quantidade atual de elementos
} HeapMinimo;

//funçoes para heap/ fila de prioridade

HeapMinimo* criarHeap(int capacidade);
void inserirHeap(HeapMinimo* heap, int vertice, double distancia);  
NoHeap removerMinimo(HeapMinimo* heap);
int heapVazio(HeapMinimo* heap);

//dijkstra com heap minimo

void dijkstra(GrafoLista* grafo, int origem, int destino);

#endif
