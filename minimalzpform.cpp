#include "minimalzpform.h"
#include "ui_minimalzpform.h"

MinimalZPForm::MinimalZPForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::MinimalZPForm)
{
    ui->setupUi(this);
}

MinimalZPForm::~MinimalZPForm()
{
    delete ui;
}

void MinimalZPForm::on_pushButton_2_clicked()
{
    this->done(-1);
}

void MinimalZPForm::on_pushButton_clicked()
{
    bool flag = false;
    this->min_zp = ui->lineEdit->text().toDouble(&flag);
    if (flag==false)
        QMessageBox::warning(this, "Ошибка", "Невозможно преобразовать в число!");
    else
        this->done(1);
}

void MinimalZPForm::showEvent(QShowEvent *event)
{
    ui->lineEdit->setText(QString::number(this->min_zp));
}
