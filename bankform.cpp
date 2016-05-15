#include "bankform.h"
#include "ui_bankform.h"

BankForm::BankForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::BankForm)
{
    ui->setupUi(this);
}

BankForm::~BankForm()
{
    delete ui;
}

void BankForm::Init()
{
    bank_model = new QSqlTableModel(this);
    bank_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    bank_model->setTable("bank");
    bank_model->setHeaderData(1, Qt::Horizontal, "Наименование");
    //bank_model->setSort(1, Qt::AscendingOrder);
    ui->tableBank->setModel(bank_model);
    ui->tableBank->hideColumn(0);
    bank_model->select();
    ui->tableBank->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void BankForm::on_action_exit_triggered()
{
    close();
}

void BankForm::on_action_add_triggered()
{
    bank_model->insertRow(bank_model->rowCount());
}

void BankForm::on_action_del_triggered()
{
    bank_model->removeRow(ui->tableBank->currentIndex().row());
}

void BankForm::on_action_save_triggered()
{
    if (!bank_model->submitAll())
    {
        QMessageBox::information(this, "Невозможно сохранить изменения", bank_model->lastError().text());
    }
    ui->tableBank->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}
