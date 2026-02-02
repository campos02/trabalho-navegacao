#ifndef TRIE_H
#define TRIE_H

#include <stdbool.h>
#include <stdint.h>
#include <QFile>

#define TAMANHO_MAX_NOME 256 // Tamanho máximo de um nome de rua

// Estrutura de um nó da Trie
typedef struct NoTrie
{
    struct NoTrie *filhos[256]; // Para caracteres ASCII (0-255)
    bool fim_da_palavra;
    uint64_t *ids; // Array de IDs associados
    int quantidade_ids;
    int capacidade_ids;
} NoTrie;

// Estrutura principal da Trie
typedef struct
{
    NoTrie *raiz;
    int total_palavras; // Para estatísticas
} ArvoreTrie;

ArvoreTrie *criar_trie(void);

void inserir_na_trie(ArvoreTrie *trie,
                     const char *palavra,
                     uint64_t id); // Insere uma palavra na Trie com seu ID associado

char **buscar_prefixo_trie(ArvoreTrie *trie, const char *prefixo, int *quantidade);

uint64_t *buscar_ids_trie(
    ArvoreTrie *trie,
    const char *palavra,
    int *quantidade_ids); // Busca uma palavra exata e retorna seus IDs associados

void liberar_trie(ArvoreTrie *trie);

void mostrar_estatisticas_trie(ArvoreTrie *trie); // Exibe estatísticas da Trie (para debug)

void carregar_nodes_to_label(ArvoreTrie *trie, QFile *file); // Lê nodes_to_label.json e insere na Trie

#endif // TRIE_H
