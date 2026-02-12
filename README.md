# trabalho-navegacao

Disciplina de Algoritmo de Estrutura de Dados II

Alunos:
LARISSA GABRIELA BARROZO
VINICIUS BOHM DE CAMPOS
RAPHAEL AMBROSIO BEZERRA


# Rodando no Qt Creator
1. Clonar o repositório com git
2. No Qt Creator clicar em Open Project e dentro do diretório do projeto selecionar CMakeLists.txt
3. Após configurar os kits habilitar a opção Run in Terminal e utilizar a ferramenta Run


Descrição resumida das estruturas:

### vertice.cpp:

trecho codigo 1: 

int indice_por_id(Vertices *vertices, uint64_t id)
{
    int indice = id % vertices->quantidade;

    if (vertices->vertices[indice].id != id)
        indice /= 2;

    while (indice < vertices->quantidade - 1 &&
           vertices->vertices[indice].id != id)
        indice++;

    return indice;
}

Objetivo : Encontrar o índice do vértice no vetor a partir do id
Calcula um índice inicial usando: id % quantidade e funciona como hash simples

Se houver colisão: divide o índice por 2 (tentativa de ajuste) e depois faz busca linear até achar o id.

Complexidade: 
Caso ideal (sem colisão): O(1)
Caso médio ( Pequenas colisões) : O(k)
Pior caso: O(n)

### TRIE.CPP

trecho de codigo 1:

typedef struct NoTrie {
    struct NoTrie *filhos[256];
    bool fim_da_palavra;

    uint64_t *ids;
    int quantidade_ids;
    int capacidade_ids;

} NoTrie;

Objetivo: Representa um nó da Trie.

Complexidade: 
Acesso a filho por caractere: O(1)
Inserção de id (amortizado): O(1)

trecho codigo 2: arvore trie

typedef struct {
    NoTrie *raiz;
    int total_palavras;
} ArvoreTrie;

Objetivo: Representa a Trie inteira.
raiz → ponto inicial da árvore.
total_palavras → contador de nomes distintos inseridos.

trecho codigo 3: criar nó

static NoTrie *criar_no_trie(void)
{
    NoTrie *no = (NoTrie *) malloc(sizeof(NoTrie));

    for (int i = 0; i < 256; i++)
        no->filhos[i] = NULL;

    no->fim_da_palavra = false;
    no->ids = NULL;
    no->quantidade_ids = 0;
    no->capacidade_ids = 0;

    return no;
}
Complexidade: O(256) → O(1)

Trecho codigo 4: inserir em trie 

void inserir_na_trie(ArvoreTrie *trie, const char *palavra, uint64_t id)
{
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
Descrição
Percorre caractere por caractere.
Cria nós conforme necessário.
Marca fim da palavra e Adiciona id ao vetor do nó final.

Complexidade: O(k) , onde k é tamanho da palavra

trecho codigo: buscar prefixo

Resultado *buscar_prefixo_trie(ArvoreTrie *trie,
                               const char *prefixo,
                               int *quantidade)
{
    NoTrie *atual = trie->raiz;

    for (int i = 0; prefixo[i] != '\0'; i++) {
        unsigned char c = (unsigned char) prefixo[i];
        if (!atual->filhos[c]) {
            *quantidade = 0;
            return NULL;
        }
        atual = atual->filhos[c];
    }

    int limite = 10;
    Resultado *resultados =
        (Resultado *) malloc(limite * sizeof(Resultado));

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
Descrição: 
Percorre o prefixo.
Encontra o nó correspondente.
Coleta até 10 sugestões.

Trecho codigo: Busca de palavra completa na trie

uint64_t *buscar_ids_trie(ArvoreTrie *trie,
                          const char *palavra,
                          int *quantidade_ids)
{
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
Objetivo: Percorre palavra completa. Se existir, retorna vetor interno de ids.

Complexidade: O(k)


## dijkstra.cpp:




