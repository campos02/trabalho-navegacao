# Sistema de Navegação Viária com Autocomplete e Caminho Mínimo

Disciplina de Algoritmo de Estrutura de Dados II
Trabalho final

Alunos:
LARISSA GABRIELA BARROZO
VINICIUS BOHM DE CAMPOS
RAPHAEL AMBROSIO BEZERRA


# Rodando no Qt Creator
1. Clonar o repositório com git
2. No Qt Creator clicar em Open Project e dentro do diretório do projeto selecionar CMakeLists.txt
3. Após configurar os kits habilitar a opção Run in Terminal e utilizar a ferramenta Run


# Descrição resumida das estruturas:

# vertice.cpp:

trecho codigo 1: 

```c
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
```

Objetivo : Encontrar o índice do vértice no vetor a partir do id
- Calcula um índice inicial usando:  id % quantidade 
- funciona como hash simples
- Se houver colisão: divide o índice por 2 (tentativa de ajuste) e depois faz busca linear até achar o id.

Complexidade: 
Caso ideal (sem colisão): O(1)
Caso médio ( Pequenas colisões) : O(k)
Pior caso: O(n)

### TRIE.CPP

trecho de codigo:

```c
typedef struct NoTrie {
    struct NoTrie *filhos[256];
    bool fim_da_palavra;

    uint64_t *ids;
    int quantidade_ids;
    int capacidade_ids;

} NoTrie;
```

Objetivo: Representa um nó da Trie.

Complexidade: 
Acesso a filho por caractere: O(1)
Inserção de id (amortizado): O(1)

trecho codigo: arvore trie

```c
typedef struct {
    NoTrie *raiz;
    int total_palavras;
} ArvoreTrie;
```

Objetivo: Representa a Trie inteira.
raiz → ponto inicial da árvore.
total_palavras → contador de nomes distintos inseridos.

trecho codigo: criar nó -> Complexidade: O(1)

```c
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
```

Trecho codigo: inserir em trie 

```c
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
```

Objetivo do codigo acima: 
Percorrer caractere por caractere.
Criar nós conforme necessário.
Marcar fim da palavra e Adiciona id ao vetor do nó final.

Complexidade: O(k) , onde k é tamanho da palavra

trecho codigo:
buscar prefixo:

```c
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
```

Trecho codigo: Busca de palavra completa na trie -> Complexidade: O(k)

```c
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
```

Objetivo: Percorre palavra completa. Se existir, retorna vetor interno de ids.

## dijkstra.cpp:

trechos de HeapMinimo:

Representa a fila de prioridade (min-heap) usada no Dijkstra.

```c
HeapMinimo *heap = criar_heap(n); -> Complexidade O(n) 
 
inserir_heap(heap, origem, 0.0); -> Insere no final e Sobe ajustando propriedade do heap. -> Complexidade O(log n)

NoHeap atual_heap = extrair_minimo(heap);  ->  Complexidade O(log n)

trecho distancia: 

Distancia *distancias =
    (Distancia *) malloc(n * sizeof(Distancia));

distancias[i].distancia = INT_MAX / 2;
distancias[i].predecessor = 0;
```

onde: distancia → menor distância encontrada
      predecessor → vértice anterior no caminho


trecho codigo: Distancia *calcular_distancias_dijkstra(Vertices *vertices, uint64_t origem)

* como código é muito grande, não foi postado aqui.
* Mas: 

Objetivo: 
Calcular a menor distância da origem para todos os outros vértices usando Dijkstra.
Retorna um vetor Distancia onde, para cada vértice:
distancia → menor custo encontrado
predecessor → vértice anterior no menor caminho

1. inicializa, Criando vetor de tamanho V (número de vértices).
2. Todos começam como "não alcançados" e Marca origem com distância 0 (inserir_heap(heap, origem, 0.0);)
3. Heap controla qual vértice será processado primeiro (menor distância).
4. Enquanto existirem vértices para processar: Extrai menor distância com NoHeap atual_heap = extrair_minimo(heap);
5. Marca como processado: processado[indice_atual] = 1;
6. Relaxamento das arestas: for (int i = 0; i < quantidade_arestas; i++) e ve os vizinhos
7. Se o novo caminho for menor: if (nova_distancia < distancias[indice_destino].distancia)
8. Atualiza

resumindo, a função devolve um vetor distancias onde:

Para qualquer vértice X
distancias[X].distancia → menor distância desde origem
distancias[X].predecessor → vértice anterior no caminho ótimo

Complexidade: O((V + E) log V)


Trecho: construir_caminho_dijkstra

```c
Caminho construir_caminho_dijkstra(Vertices *vertices,
                                   uint64_t origem,
                                   uint64_t destino,
                                   Distancia *distancias)
```
                                   
Objetivo: Reconstruir o caminho mínimo da origem até o destino, usando os predecessores calculados antes.

## MAIN.CPP

O main está dividido em partes:
- Inicialização dos dados
- interface de busca da origem e seleção
-interface de busca de destino
- calculo e exibir caminho

observação: A interface é em terminal

1. Carrega o grafo (vértices + arestas) e Monta a Trie com os nomes
2. escolhe origem e usa buscar_prefixo_trie para então mostrar os resultados com: resultados_origem[opcao_resultado_origem].id, onde esse id é o índice do vértice no grafo.
3. escolhe destino e usa da mesma logica e no final mostra os resultados com: resultados_destino[opcao_resultado_destino].id

4. Dijkstra entra aqui :

```c
Distancia *distancias
    = calcular_distancias_dijkstra(&vertices,
        resultados_origem[opcao_resultado_origem].id);
```

onde roda Dijkstra uma única vez a partir da origem, gerando um vetor: distancias[i]
Que contém, para cada vértice: menor distância até ele e qual foi o vértice anterior no caminho


5.Exibição dos resultados:

```c
for (int j = 0; j < quantidade_resultados; j++)
    printf("%d. %s\n", j + 1, resultados_origem[j].nome);
```

# observação: 

```text
Lógica de exibição:

primary / secondary → Avenida
resto               → Rua
```


```c
if (!strcmp(aresta.tipo, "primary") || !strcmp(aresta.tipo, "secondary"))
    printf("Av. %s -> ", aresta.nome);
else
    printf("R. %s -> ", aresta.nome);
```

Segue essa ideia: 
```text
Carregamento de Dados
│
▼
Interface textual (Trie)
│
▼
Seleção da origem
│
▼
Interface textual (Trie)
│
▼
Seleção do destino
│
▼
Dijkstra
│
▼
Reconstrução do caminho
│
▼
Impressão formatada
│
▼
Liberação de memória
```


## ler_dados.cpp: 

Esse arquivo é responsável por:

- Ler os arquivos JSON do mapa
- Construir o grafo na memória
- Preparar a estrutura para trie e Dijkstra

* usa principalmente os dois arquivos:

nodes.json → contém os vértices
edges.json → contém as arestas


# Observação:

Nenhum dos arquivos .h foi explicado aqui , ja que servem para declarar estruturas, funções e outros que serão usados em outros arquivos .cpp.


Observação:
nem sempre o git funcionava, então alguns dos integrantes modificava diretamente no github.
