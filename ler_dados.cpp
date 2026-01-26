#include <QFile>
#include <QByteArray>
#include <QDebug>
#include <QTextStream>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#include <stdlib.h>

#include "vertices.h"

Vertices ler_vertices(QFile* file) {
    Vertices vertices;
    vertices.quantidade = 0;
    vertices.vertices = NULL;

    file->setFileName("nodes.json");
    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << "Json file couldn't be opened/found";
        return vertices;
    }

    QByteArray byteArray = file->readAll();
    file->close();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Parse error at " << parseError.offset << ":" << parseError.errorString();
        return vertices;
    }

    QJsonArray verticesJson = jsonDoc.array();
    QTextStream textStream(stdout);

    vertices.quantidade = verticesJson.size();
    vertices.vertices = (Vertice *) malloc(vertices.quantidade * sizeof(Vertice));

    if (vertices.vertices == NULL) {
        qWarning() << "Erro ao alocar memoria para vertices";
        return vertices;
    }

    for (int i = 0; i < vertices.quantidade; i++) {
        vertices.vertices[i].id = 0;
        vertices.vertices[i].x = 0;
        vertices.vertices[i].y = 0;
    }

    for (int i = 0; i < verticesJson.size(); i++) {
        QJsonObject verticeJson = verticesJson[i].toObject();
        uint64_t id = (uint64_t) verticeJson.value("id").toInteger();
        int indice = id % vertices.quantidade;

        if (vertices.vertices[indice].id != 0)
            indice /= 2;

        while (indice < vertices.quantidade - 1 && vertices.vertices[indice].id != 0)
            indice++;

        vertices.vertices[indice].id = id;
        vertices.vertices[indice].x = verticeJson.value("x").toDouble();
        vertices.vertices[indice].y = verticeJson.value("y").toDouble();

        vertices.vertices[indice].quantidade_arestas = verticeJson.value("street_count").toInt();
        vertices.vertices[indice].arestas = (Aresta*) malloc(vertices.vertices[indice].quantidade_arestas * sizeof(Aresta));

        if (vertices.vertices[indice].arestas == NULL) {
            qWarning() << "Erro ao alocar memoria para arestas";
            return vertices;
        }

        for (int j = 0; j < vertices.vertices[indice].quantidade_arestas; j++) {
            vertices.vertices[indice].arestas[j].osmid = 0;
            vertices.vertices[indice].arestas[j].vertice = 0;
            vertices.vertices[indice].arestas[j].tamanho = 0;
        }
    }

    return vertices;
}

void ler_arestas(Vertices* vertices, QFile* file) {
    file->setFileName("edges.json");
    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << "Json file couldn't be opened/found";
        return;
    }

    QByteArray byteArray = file->readAll();
    file->close();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Parse error at " << parseError.offset << ":" << parseError.errorString();
        return;
    }

    QJsonArray arestasJson = jsonDoc.array();
    QTextStream textStream(stdout);

    for (int i = 0; i < arestasJson.size(); i++) {
        QJsonObject arestaJson = arestasJson[i].toObject();
        uint64_t vertice = (uint64_t) arestaJson.value("u").toInteger();

        int indice_vertice = indice_por_id(vertices, vertice);
        int indice_aresta = 0;

        // Encontrar primeira posição livre
        while (indice_aresta < vertices->vertices[indice_vertice].quantidade_arestas - 1 &&
               vertices->vertices[indice_vertice].arestas[indice_aresta].vertice != 0)
            indice_aresta++;

        vertices->vertices[indice_vertice].arestas[indice_aresta].vertice = (uint64_t) arestaJson.value("v").toInteger();

        QJsonObject dadosJson = arestaJson.value("data").toObject();
        vertices->vertices[indice_vertice].arestas[indice_aresta].osmid = (uint64_t) dadosJson.value("osmid").toInteger();
        vertices->vertices[indice_vertice].arestas[indice_aresta].tamanho = dadosJson.value("length").toDouble();
    }
}
