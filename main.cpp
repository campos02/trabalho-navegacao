#include <QFile>
#include <QDebug>
#include <QByteArray>
#include <QTextStream>

#include <QJsonParseError>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

#include <stdlib.h>
#include "cidade.h"

int main(int argc, char *argv[])
{
    QFile file;
    file.setFileName("nodes.json");

    if (!file.open(QIODevice::ReadOnly)) {
        qDebug() << "Json file couldn't be opened/found";
        return 1;
    }

    QByteArray byteArray = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(byteArray, &parseError);

    if (parseError.error != QJsonParseError::NoError) {
        qWarning() << "Parse error at " << parseError.offset << ":" << parseError.errorString();
        return 1;
    }

    QJsonArray cidadesJson = jsonDoc.array();
    QTextStream textStream(stdout);

    Cidade* cidades = (Cidade*)malloc(cidadesJson.size() * sizeof(Cidade));

    for (int i = 0; i < cidadesJson.size(); i++) {
        QJsonObject cidadeJson = cidadesJson[i].toObject();
        cidades[i].id = cidadeJson.value("id").toInt();
        cidades[i].x = cidadeJson.value("x").toDouble();
        cidades[i].y = cidadeJson.value("y").toDouble();

        textStream << cidades[i].id << Qt::endl;
        textStream << cidades[i].x << Qt::endl;
        textStream << cidades[i].y << Qt::endl << Qt::endl;
    }

    free(cidades);
}
