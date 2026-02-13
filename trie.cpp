#include "trie.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>
#include <QDebug>
//  Funções auxiliares 

static NoTrie *criar_no_trie(void)
{
    NoTrie *no = (NoTrie *) malloc(sizeof(NoTrie));
    if (!no)
        return NULL;

    for (int i = 0; i < 256; i++)
        no->filhos[i] = NULL;

    no->fim_da_palavra = false;
    no->ids = NULL;
    no->quantidade_ids = 0;
    no->capacidade_ids = 0;

    return no;
}

static void adicionar_id(NoTrie *no, uint64_t id)
{
    if (no->quantidade_ids >= no->capacidade_ids) {
        no->capacidade_ids = (no->capacidade_ids == 0) ? 2 : no->capacidade_ids * 2;
        no->ids = (uint64_t *) realloc(no->ids,
                                       no->capacidade_ids * sizeof(uint64_t));
    }

    no->ids[no->quantidade_ids++] = id;
}

//API pública

ArvoreTrie *criar_trie(void)
{
    ArvoreTrie *trie = (ArvoreTrie *) malloc(sizeof(ArvoreTrie));
    if (!trie)
        return NULL;

    trie->raiz = criar_no_trie();
    trie->total_palavras = 0;

    return trie;
}

void inserir_na_trie(ArvoreTrie *trie, const char *palavra, uint64_t id)
{
    if (!trie || !palavra)
        return;

    NoTrie *atual = trie->raiz;

    for (int i = 0; palavra[i] != '\0'; i++) {
        unsigned char c = (unsigned char) palavra[i];

        if (!atual->filhos[c])
            atual->filhos[c] = criar_no_trie();

        atual = atual->filhos[c];
    }

    if (!atual->fim_da_palavra)
        trie->total_palavras++;

    atual->fim_da_palavra = true;
    adicionar_id(atual, id);
}

static void coletar_palavras(
    NoTrie *no, char *buffer, int profundidade, Resultado *resultados, int *contador, int limite)
{
    if (!no || *contador >= limite)
        return;

    if (no->fim_da_palavra) {
        buffer[profundidade] = '\0';

        // Como é o fim de uma palavra ela tem um ID só
        resultados[*contador].id = no->ids[0];
        resultados[*contador].nome = strdup(buffer);
        (*contador)++;
    }

    for (int i = 0; i < 256 && *contador < limite; i++) {
        if (no->filhos[i]) {
            buffer[profundidade] = (char) i;
            coletar_palavras(no->filhos[i],
                              buffer,
                              profundidade + 1,
                              resultados,
                              contador,
                              limite);
        }
    }
}

Resultado *buscar_prefixo_trie(ArvoreTrie *trie, const char *prefixo, int *quantidade)
{
    if (!trie || !prefixo) {
        *quantidade = 0;
        return NULL;
    }

    NoTrie *atual = trie->raiz;

    for (int i = 0; prefixo[i] != '\0'; i++) {
        unsigned char c = (unsigned char) prefixo[i];
        if (!atual->filhos[c]) {
            *quantidade = 0;
            return NULL;
        }
        atual = atual->filhos[c];
    }

    int limite = 10; // limite de sugestões
    
    Resultado *resultados = (Resultado *) malloc(limite * sizeof(Resultado));
    char buffer[TAMANHO_MAX_NOME];

    strcpy(buffer, prefixo);
    *quantidade = 0;

    coletar_palavras(atual,
                     buffer,
                     strlen(prefixo),
                     resultados,
                     quantidade,
                     limite);

    return resultados;
}

uint64_t *buscar_ids_trie(ArvoreTrie *trie, const char *palavra, int *quantidade_ids)
{
    if (!trie || !palavra) {
        *quantidade_ids = 0;
        return NULL;
    }

    NoTrie *atual = trie->raiz;

    for (int i = 0; palavra[i] != '\0'; i++) {
        unsigned char c = (unsigned char) palavra[i];
        if (!atual->filhos[c]) {
            *quantidade_ids = 0;
            return NULL;
        }
        atual = atual->filhos[c];
    }

    if (!atual->fim_da_palavra) {
        *quantidade_ids = 0;
        return NULL;
    }

    *quantidade_ids = atual->quantidade_ids;
    return atual->ids;
}

static void liberar_no_trie(NoTrie *no)
{
    if (!no)
        return;

    for (int i = 0; i < 256; i++)
        liberar_no_trie(no->filhos[i]);

    free(no->ids);
    free(no);
}

void liberar_trie(ArvoreTrie *trie)
{
    if (!trie)
        return;

    liberar_no_trie(trie->raiz);
    free(trie);
}

void carregar_nodes_to_label(ArvoreTrie *trie, QFile *file)
{
    if (!trie || !file)
        return;

    file->setFileName("nodes_to_label.json");

    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << "Erro ao abrir nodes_to_label.json";
        return;
    }

    QByteArray data = file->readAll();
    file->close();

    QJsonParseError erro;
    QJsonDocument doc = QJsonDocument::fromJson(data, &erro);

    if (erro.error != QJsonParseError::NoError) {
        qDebug() << "Erro JSON:" << erro.errorString();
        return;
    }

    QJsonObject obj = doc.object();

    for (auto it = obj.begin(); it != obj.end(); ++it) {
        uint64_t id = it.key().toULongLong();
        QString nome = it.value().toString();

        inserir_na_trie(trie,
                        nome.toUtf8().constData(),
                        id);
    }
}
void mostrar_estatisticas_trie(ArvoreTrie *trie)
{
    if (!trie)
        return;

    printf("Total de palavras na Trie: %d\n", trie->total_palavras);
}
