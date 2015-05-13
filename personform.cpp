#include "personform.h"
#include "ui_personform.h"

PersonForm::PersonForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::PersonForm)
{
    ui->setupUi(this);
}

PersonForm::~PersonForm()
{
    delete ui;
}

void PersonForm::Init()
{
    persona_model = new QSqlTableModel(this);
    persona_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    persona_model->setTable("persona");
    persona_model->setHeaderData(1, Qt::Horizontal, "ФИО");
    //persona_model->setSort(1, Qt::AscendingOrder);
    ui->tablePersona->setModel(persona_model);
    ui->tablePersona->hideColumn(0);
    ui->tablePersona->connect(ui->tablePersona->selectionModel(), SIGNAL(currentRowChanged(QModelIndex,QModelIndex)), this, SLOT(on_tablePersona_changed(QModelIndex)));
    persona_model->select();
    ui->tablePersona->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);

    schet_model = new QSqlRelationalTableModel(this);
    schet_model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    schet_model->setTable("schet");
    schet_model->setHeaderData(1, Qt::Horizontal, "Банк");
    schet_model->setHeaderData(2, Qt::Horizontal, "Код вклада");
    schet_model->setHeaderData(3, Qt::Horizontal, "Лицевой счет");
    schet_model->setRelation(1, QSqlRelation("bank", "id", "name"));
    schet_model->relationModel(1)->setSort(1, Qt::AscendingOrder);
    schet_model->relationModel(1)->select();
    //schet_model->setSort(1, Qt::AscendingOrder);
    ui->tableSchet->setModel(schet_model);
    ui->tableSchet->setItemDelegate(new QSqlRelationalDelegate(ui->tableSchet));
    ui->tableSchet->hideColumn(0);
    ui->tableSchet->hideColumn(4);
    schet_model->select();
    ui->tableSchet->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void PersonForm::on_tablePersona_changed(QModelIndex index)
{
    if (index.isValid())
    {
        QSqlRecord rec = persona_model->record(index.row());
        schet_model->setFilter("id_persona="+rec.value(0).toString());
    }else
    {
        schet_model->setFilter("id_persona=-1");
    }
    schet_model->select();
}

void PersonForm::on_action_exit_triggered()
{
    close();
}

void PersonForm::on_action_add_person_triggered()
{
    persona_model->insertRow(0);
    ui->tablePersona->selectRow(0);
}

void PersonForm::on_action_del_person_triggered()
{
    persona_model->removeRow(ui->tablePersona->currentIndex().row());
}

void PersonForm::on_action_save_person_triggered()
{
    if (!persona_model->submitAll())
    {
        QMessageBox::information(this, "Невозможно сохранить изменения", persona_model->lastError().text());
    }
    ui->tablePersona->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void PersonForm::on_action_add_schet_triggered()
{
    if (ui->tablePersona->currentIndex().row()==-1)
    {
        QMessageBox::information(this,"Сообщение","Добавление невозможно!\nНе выбран сотрудник");
    }
    else
    {
        schet_model->insertRow(schet_model->rowCount());
        schet_model->setData(schet_model->index(schet_model->rowCount()-1, 4), persona_model->record(ui->tablePersona->currentIndex().row()).value("id"));
    }
}

void PersonForm::on_action_del_schet_triggered()
{
    schet_model->removeRow(ui->tableSchet->currentIndex().row());
}

void PersonForm::on_action_save_schet_triggered()
{
    if (!schet_model->submitAll())
    {
        QMessageBox::information(this, "Невозможно сохранить изменения", schet_model->lastError().text());
    }
    ui->tableSchet->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}
