#include "mainform.h"
#include "paylistbelarusbank.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainForm w;
    //PayListBelarusbank w;
    w.show();

    return a.exec();
}
