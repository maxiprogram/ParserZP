#ifndef PARSER_H
#define PARSER_H

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QTextCodec>

#include <QDebug>

struct DataRecord
{
    QString fio;
    QString summa;
};

class Parser
{
public:
    Parser();
    bool Load(QString filename);
    QList<DataRecord> GetList();
    double GetSumma();
private:
    QList<DataRecord> list;
    double summa;
};

#endif // PARSER_H
