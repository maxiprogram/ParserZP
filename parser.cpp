#include "parser.h"

Parser::Parser()
{
}

bool Parser::Load(QString filename)
{
    list.clear();
    summa = 0;
    QFile in_f(filename);
    if (!in_f.open(QIODevice::ReadOnly))
        return false;
    QTextStream in_text;
    in_text.setDevice(&in_f);
    in_text.setCodec(QTextCodec::codecForName("CP866"));

    while (!in_text.atEnd())
    {
        QString tmp_str = in_text.readLine();
        if (tmp_str.size()>=3)
        {
            if (tmp_str.at(0)==' ' && tmp_str.at(1)=='0' && tmp_str.at(2)=='9')
            {
                DataRecord tmp_data;
                tmp_data.fio = tmp_str.right(tmp_str.size()-3);
                in_text.readLine();
                tmp_str = in_text.readLine();
                tmp_data.summa = tmp_str.right(tmp_str.size()-3);
                list.append(tmp_data);
                summa += tmp_data.summa.toDouble();
                //qDebug()<<tmp_data.fio<<tmp_data.summa;
            }
        }
    }

    in_f.close();
    return true;
}

QList<DataRecord> Parser::GetList()
{
    return list;
}

double Parser::GetSumma()
{
    return summa;
}
