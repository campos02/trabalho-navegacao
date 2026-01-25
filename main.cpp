#include "ler_dados.h"
#include <stdlib.h>

int main(int argc, char *argv[])
{
    QFile file;
    Cidades cidades = ler_cidades(&file);
    free(cidades.cidades);
}
