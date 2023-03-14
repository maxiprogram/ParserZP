#include "paylistbelarusbank.h"
#include "ui_paylistbelarusbank.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

PayListBelarusbank::PayListBelarusbank(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PayListBelarusbank)
{
    ui->setupUi(this);
    /*QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    qDebug()<<db.drivers();
    db.setDatabaseName("Driver={Microsoft Access Driver (*.mdb, *.accdb)};Dbq=D:\\Constantine\\MyProject\\Release\\ParserZP\\ParserZP-master\\db\\database.mdb;Uid=;Pwd=;");
    //qDebug()<<db.open();
    if (!db.open())
        QMessageBox::warning(this, "Ошибка подключения", db.lastError().text());*/

}

PayListBelarusbank::~PayListBelarusbank()
{
    delete ui;
}

void PayListBelarusbank::init()
{
    QSqlQuery queryPayListBelarusbank(QSqlDatabase::database());
    queryPayListBelarusbank.exec("SELECT * FROM PayListBelarusbank");
    queryPayListBelarusbank.next();
    ui->editNsp->setText(queryPayListBelarusbank.value("NSP").toString());
    ui->editNotd->setText(queryPayListBelarusbank.value("NOTD").toString());
    ui->editNfil->setText(queryPayListBelarusbank.value("NFIL").toString());
    //ui->editAcc->setText(queryPayListBelarusbank.value("ACC").toString());
    ui->comboBoxAcc->clear();
    QString tmp_acc = queryPayListBelarusbank.value("ACC").toString();
    QStringList list = tmp_acc.split(";");
    foreach (QString schet, list)
        ui->comboBoxAcc->addItem(schet);
    ui->editFisp->setText(queryPayListBelarusbank.value("FISP").toString());
    ui->editNzp->setText(queryPayListBelarusbank.value("NZP").toString());
    ui->editNpp->setText(queryPayListBelarusbank.value("NPP").toString());
    ui->editContract->setText(queryPayListBelarusbank.value("CONTRACT").toString());
    queryPayListBelarusbank.clear();
}

void PayListBelarusbank::on_pushButton_2_clicked()
{
    this->reject();
}

void PayListBelarusbank::on_pushButton_clicked()
{
    bool isEmpty = ui->editContract->text().isEmpty()||
                   ui->editFisp->text().isEmpty()||ui->editNpp->text().isEmpty()||
                   ui->editNsp->text().isEmpty()||ui->editNzp->text().isEmpty()||
                   ui->editNotd->text().isEmpty()||ui->editNfil->text().isEmpty();
    if(isEmpty)
    {
        QMessageBox::about(this,"Сообщение!","Все поля ввода должны быть заполнены.");
        return;
    }
    /*if(ui->editAcc->text().count() != 28)
    {
        QMessageBox::about(this,"Сообщение!","Номер счета, должен содержать 28-мь символов.");
        return;
    }*/
    QString updateTable = QString("UPDATE PayListBelarusbank SET NSP='%1', NOTD='%2',"
                                   " NFIL='%3', ACC='%4', FISP='%5',"
                                   " NZP='%6', NPP='%7', CONTRACT='%8' "
                                   "WHERE ID=1;")
                                    .arg(ui->editNsp->text()).arg(ui->editNotd->text())
                                    .arg(ui->editNfil->text()).arg(ui->comboBoxAcc->currentText())
                                    .arg(ui->editFisp->text()).arg(ui->editNzp->text())
                                    .arg(ui->editNpp->text()).arg(ui->editContract->text());
    qDebug()<<updateTable;
    QSqlQuery queryPayListBelarusbank(QSqlDatabase::database());
    qDebug()<<queryPayListBelarusbank.exec(updateTable);
    qDebug()<<queryPayListBelarusbank.lastError().text();
    queryPayListBelarusbank.clear();
    this->accept();
}
