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
}

void InsertActiveForm::on_pushButton_2_clicked()
{
    this->close();
}

void InsertActiveForm::on_pushButton_clicked()
{
    emit ADD_RECORD(ui->comboBox->currentData().toString(), ui->comboBox_2->currentData().toString(), ui->lineEdit->text());
    this->close();
}
