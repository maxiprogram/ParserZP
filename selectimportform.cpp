#include "selectimportform.h"
#include "ui_selectimportform.h"

SelectImportForm::SelectImportForm(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SelectImportForm)
{
    ui->setupUi(this);
}

SelectImportForm::~SelectImportForm()
{
    delete ui;
}

void SelectImportForm::on_pushButton_2_clicked()
{
    this->done(0);
}

void SelectImportForm::on_pushButton_clicked()
{
    this->done(ui->comboBox->currentIndex()+1);
}
