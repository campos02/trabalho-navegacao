#include <QFile>
#include <QByteArray>
#include <QDebug>
#include <QTextStream>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

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

    for (int i = 0; i < vertices.quantidade; i++) {
        vertices.vertices[i].id = 0;
        vertices.vertices[i].x = 0;
        vertices.vertices[i].y = 0;
    }

    for (int i = 0; i < verticesJson.size(); i++) {
        QJsonObject verticeJson = verticesJson[i].toObject();
        uint64_t id = (uint64_t) verticeJson.value("id").toInteger();
        int indice = indice_por_id(&vertices, id);

        vertices.vertices[indice].id = id;
        vertices.vertices[indice].x = verticeJson.value("x").toDouble();
        vertices.vertices[indice].y = verticeJson.value("y").toDouble();
    }

    return vertices;
}
