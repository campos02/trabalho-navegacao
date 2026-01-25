#include "ler_dados.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    QFile file;
    Vertices vertices = ler_vertices(&file);
    free(vertices.vertices);
}
