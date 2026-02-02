#ifndef LER_DADOS_H
#define LER_DADOS_H

#include <QFile>
#include "vertices.h"

Vertices ler_vertices(QFile *file);
void ler_arestas(Vertices *vertices, QFile *file);

#endif // LER_DADOS_H
