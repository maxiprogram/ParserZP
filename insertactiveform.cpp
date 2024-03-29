#include "insertactiveform.h"
#include "ui_insertactiveform.h"

InsertActiveForm::InsertActiveForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::InsertActiveForm)
{
    ui->setupUi(this);
}

InsertActiveForm::~InsertActiveForm()
{
    delete ui;
}

void InsertActiveForm::Init()
{
    QString query_string = "SELECT id, fio FROM persona ORDER BY fio ASC;";
    QSqlQuery query(QSqlDatabase::database());
    if (!query.exec(query_string))
    {
        QMessageBox::warning(this, "Ошибка запроса", query.lastError().text());
        return;
    }
    ui->comboBox->clear();
    if (query.isSelect())
        query.first();
    if (query.isValid())
    {
        do
        {
            ui->comboBox->addItem(query.value(1).toString(), query.value(0));
        }while(query.next());
    }

    query_string = "SELECT id, name FROM bank ORDER BY name ASC;";
    query.clear();
    if (!query.exec(query_string))
    {
        QMessageBox::warning(this, "Ошибка запроса", query.lastError().text());
        return;
    }
    ui->comboBox_2->clear();
    if (query.isSelect())
        query.first();
    if (query.isValid())
    {
        do
        {
            ui->comboBox_2->addItem(query.value(1).toString(), query.value(0));
        }while(query.next());
    }
    ui->lineEdit->setText("");
    ui->lineEdit_search->setText("");
}

void InsertActiveForm::on_pushButton_2_clicked()
{
    this->close();
}

void InsertActiveForm::on_pushButton_clicked()
{
    bool flag = false;
    ui->lineEdit->text().toDouble(&flag);
    if (flag==false)
    {
        QMessageBox::warning(this, "Ошибка", "Невозможно преобразовать cумму в число!");
        return;
    }
    emit ADD_RECORD(ui->comboBox->currentData().toString(), ui->comboBox_2->currentData().toString(), ui->lineEdit->text());
    this->close();
}



void InsertActiveForm::on_lineEdit_search_textChanged(const QString &arg1)
{
    if(arg1.isEmpty())
    {
        QString query_string = "SELECT id, fio FROM persona ORDER BY fio ASC;";
        QSqlQuery query(QSqlDatabase::database());
        if (!query.exec(query_string))
        {
            QMessageBox::warning(this, "Ошибка запроса", query.lastError().text());
            return;
        }
        ui->comboBox->clear();
        if (query.isSelect())
            query.first();
        if (query.isValid())
        {
            do
            {
                ui->comboBox->addItem(query.value(1).toString(), query.value(0));
            }while(query.next());
        }
    }else
    {
        QString query_string = "SELECT id, fio FROM persona WHERE fio like '"+arg1+"%' ORDER BY fio ASC;";
        QSqlQuery query(QSqlDatabase::database());
        if (!query.exec(query_string))
        {
            QMessageBox::warning(this, "Ошибка запроса", query.lastError().text());
            return;
        }
        ui->comboBox->clear();
        if (query.isSelect())
            query.first();
        if (query.isValid())
        {
            do
            {
                ui->comboBox->addItem(query.value(1).toString(), query.value(0));
            }while(query.next());
        }
    }
}
