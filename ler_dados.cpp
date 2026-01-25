#include <QFile>
#include <QByteArray>
#include <QDebug>
#include <QTextStream>

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonParseError>

#include "cidades.h"

Cidades ler_cidades(QFile* file) {
    Cidades cidades;
    cidades.quantidade = 0;
    cidades.cidades = NULL;

    file->setFileName("nodes.json");
    if (!file->open(QIODevice::ReadOnly)) {
        qDebug() << "Json file couldn't be opened/found";
        return cidades;
    }

    QByteArray byteArray = file->readAll();
    file->close();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Parse error at " << parseError.offset << ":" << parseError.errorString();
        return cidades;
    }

    QJsonArray cidadesJson = jsonDoc.array();
    QTextStream textStream(stdout);

    cidades.quantidade = cidadesJson.size();
    cidades.cidades = (Cidade *) malloc(cidades.quantidade * sizeof(Cidade));

    for (int i = 0; i < cidadesJson.size(); i++) {
        QJsonObject cidadeJson = cidadesJson[i].toObject();
        cidades.cidades[i].id = (uint64_t) cidadeJson.value("id").toInteger();
        cidades.cidades[i].x = cidadeJson.value("x").toDouble();
        cidades.cidades[i].y = cidadeJson.value("y").toDouble();

        textStream << cidades.cidades[i].id << Qt::endl;
        textStream << cidades.cidades[i].x << Qt::endl;
        textStream << cidades.cidades[i].y << Qt::endl << Qt::endl;
    }

    return cidades;
}
