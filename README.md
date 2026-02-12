# trabalho-navegacao

Disciplina de Algoritmo de Estrutura de Dados II

Alunos:
LARISSA GABRIELA BARROZO
VINICIUS BOHM DE CAMPOS
RAPHAEL AMBROSIO BEZERRA


Instruções de como abrir e rodar o projeto no Qt Creator:

Descrição resumida das estruturas:

vertice.cpp:

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



