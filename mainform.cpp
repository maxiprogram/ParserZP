#include "mainform.h"
#include "ui_mainform.h"

MainForm::MainForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);

    setting=new QSettings("settings.ini",QSettings::IniFormat,this);

    bank_form = new BankForm(this);
    persona_form = new PersonForm(this);
    insert_form = new InsertActiveForm(this);

    insert_form->connect(insert_form, SIGNAL(ADD_RECORD(QString,QString,QString)), this, SLOT(on_add_record(QString,QString,QString)));

    QSqlDatabase db = QSqlDatabase::addDatabase("QODBC");
    //qDebug()<<db.drivers();
    db.setDatabaseName("ParserZP");
    //qDebug()<<db.open();
    if (!db.open())
        QMessageBox::warning(this, "Ошибка подключения", db.lastError().text());

    parser = new Parser();
    for_bank = 0;
    count_rec = count_rec_sel = summa = summa_sel = 0;

    ui->tableActive->connect(ui->tableActive->horizontalHeader(), SIGNAL(sortIndicatorChanged(int,Qt::SortOrder)), this, SLOT(on_header_clicked(int,Qt::SortOrder)));

    ui->tableArchiv->hideColumn(0);
    ui->action_add_record->setEnabled(true);
    ui->action_del_record->setEnabled(true);
    ui->action_import->setEnabled(true);
    ui->action_export->setEnabled(true);
    ui->action_new->setEnabled(true);

    ui->action_del_archiv->setEnabled(false);
    ui->action_del_all_archiv->setEnabled(false);
}

MainForm::~MainForm()
{
    delete setting;
    delete ui;
    delete parser;
    QSqlDatabase::database().close();
}

void MainForm::Clear(QTableWidget *table)
{
    int count = table->rowCount();
    for (int i=0; i<count; i++)
    {
        table->removeRow(0);
    }
    table->setRowCount(0);
}

void MainForm::PrintArchiv()
{
    ui->tableArchiv->setRowCount(0);
    QString query_string = "SELECT id, date, name, summa FROM archiv;";
    QSqlQuery query(QSqlDatabase::database());
    if (!query.exec(query_string))
    {
        QMessageBox::warning(this, "Ошибка запроса", query.lastError().text());
        return;
    }
    if (query.isSelect())
        query.first();
    if (query.isValid())
    {
        int i = 0;
        do
        {
            ui->tableArchiv->insertRow(i);

            ui->tableArchiv->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
            ui->tableArchiv->item(i, 0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

            ui->tableArchiv->setItem(i, 1, new QTableWidgetItem(query.value(1).toDate().toString("dd.MM.yyyy")));
            ui->tableArchiv->item(i, 1)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

            ui->tableArchiv->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
            ui->tableArchiv->item(i, 2)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

            ui->tableArchiv->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));
            ui->tableArchiv->item(i, 3)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            i++;
        }while(query.next());
    }
    ui->tableArchiv->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void MainForm::PrintArchivData(QString id)
{
    ui->tableArchivData->setRowCount(0);
    QString query_string = "SELECT persona.fio, schet.kod, schet.schet, archiv_data.summa "
            "FROM persona "
            "INNER JOIN (schet INNER JOIN archiv_data ON schet.id = archiv_data.id_schet) ON persona.id = schet.id_persona "
            "WHERE id_archiv="+id+";";
    QSqlQuery query(QSqlDatabase::database());
    if (!query.exec(query_string))
    {
        QMessageBox::warning(this, "Ошибка запроса", query.lastError().text());
        return;
    }
    if (query.isSelect())
        query.first();
    if (query.isValid())
    {
        int i = 0;
        do
        {
            ui->tableArchivData->insertRow(i);

            ui->tableArchivData->setItem(i, 0, new QTableWidgetItem(query.value(0).toString()));
            ui->tableArchivData->item(i, 0)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

            ui->tableArchivData->setItem(i, 1, new QTableWidgetItem(query.value(1).toString()));
            ui->tableArchivData->item(i, 1)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

            ui->tableArchivData->setItem(i, 2, new QTableWidgetItem(query.value(2).toString()));
            ui->tableArchivData->item(i, 2)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

            ui->tableArchivData->setItem(i, 3, new QTableWidgetItem(query.value(3).toString()));
            ui->tableArchivData->item(i, 3)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            i++;
        }while(query.next());
    }
    ui->tableArchivData->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
}

void MainForm::on_header_clicked(int index, Qt::SortOrder asc)
{
    if (index==0)
    {
        if (asc==Qt::AscendingOrder)
        {
            //QMessageBox::information(this, "asc", QString::number(index));
            for (int i=0; i<ui->tableActive->rowCount(); i++)
            {
                ui->tableActive->item(i, 0)->setCheckState(Qt::Checked);
            }
        }
        else
        {
            //QMessageBox::information(this, "desc", QString::number(index));
            for (int i=0; i<ui->tableActive->rowCount(); i++)
            {
                ui->tableActive->item(i, 0)->setCheckState(Qt::Unchecked);
            }
        }
    }
}

void MainForm::on_action_bank_triggered()
{
    bank_form->Init();
    bank_form->show();
}

void MainForm::on_action_persona_triggered()
{
    persona_form->Init();
    persona_form->show();
}

void MainForm::on_action_import_triggered()
{
    ui->label_bank->setText("Формирование списка для:");
    SelectImportForm f;
    int bank = f.exec();
    //qDebug()<<for_bank;
    if (!bank)
        return;
    for_bank = bank;

    QString filename=QFileDialog::getOpenFileName(this, "Выбор файла для импорта", "in", "Список (*.txt)");
    if (filename!="" && !parser->Load(filename))
    {
        QMessageBox::warning(this, "Импорт", "Не удалось импортировать данные!");
        return;
    }

    QString query_string;
    switch(for_bank)
    {
        case 1:
        {
            query_string = "SELECT persona.fio, schet.kod, schet.schet, schet.id_bank, persona.id  FROM persona "
                       "INNER JOIN schet on schet.id_persona=persona.id "
                       "WHERE schet.id_bank=(SELECT id FROM bank WHERE name='БеларусБанк') and persona.fio=";
            ui->label_bank->setText("Формирование списка для: БеларусБанк");
            break;
        }
        case 2:
        {
            query_string = "SELECT persona.fio, schet.kod, schet.schet, schet.id_bank, persona.id  FROM persona "
                       "INNER JOIN schet on schet.id_persona=persona.id "
                       "WHERE schet.id_bank=(SELECT id FROM bank WHERE name='БелАгроПромБанк') and persona.fio=";
            ui->label_bank->setText("Формирование списка для: БелАгроПромБанк");
            break;
        }
        case 3:
        {
            query_string = "SELECT persona.fio, schet.kod, schet.schet, schet.id_bank, persona.id  FROM persona "
                       "INNER JOIN schet on schet.id_persona=persona.id "
                       "WHERE schet.id_bank=(SELECT id FROM bank WHERE name='ПриорБанк') and persona.fio=";
            ui->label_bank->setText("Формирование списка для: ПриорБанк");
            break;
        }
        case 4:
        {
            query_string = "SELECT persona.fio, schet.kod, schet.schet, schet.id_bank, persona.id  FROM persona "
                       "INNER JOIN schet on schet.id_persona=persona.id "
                       "WHERE schet.id_bank=(SELECT id FROM bank WHERE name='БеларусБанк') and persona.fio=";
            ui->label_bank->setText("Формирование списка для: Минимальная ЗП БеларусБанк");
            break;
        }
        case 5:
        {
            query_string = "SELECT persona.fio, schet.kod, schet.schet, schet.id_bank, persona.id  FROM persona "
                       "INNER JOIN schet on schet.id_persona=persona.id "
                       "WHERE schet.id_bank=(SELECT id FROM bank WHERE name='БелАгроПромБанк') and persona.fio=";
            ui->label_bank->setText("Формирование списка для: Минимальная ЗП БелАгроПромБанк");
            break;
        }
    }

    count_rec = count_rec_sel = summa = summa_sel = 0;
    ui->tableActive->setRowCount(0);
    QSqlQuery query(QSqlDatabase::database());

    QList<DataRecord> list = parser->GetList();
    for (int i=0; i<list.size(); i++)
    {
        int summa_oplat = 0;
        //qDebug()<<"SQL:"<<query_string+"'"+list.at(i).fio+"';";
        query.clear();
        if (!query.exec(query_string+"'"+list.at(i).fio+"';"))
        {
            QMessageBox::warning(this, "Ошибка в запросе", query.lastError().text());
            return;
        }

        query.first();
        if (query.isValid())
        {
            int id_persona = query.value(4).toInt();
            QSqlQuery dop_query(QSqlDatabase::database());
            dop_query.exec("SELECT id, id_bank FROM schet WHERE id_persona="+QString::number(id_persona)+" ORDER BY id_bank ASC;");
            dop_query.first();
            QVector<int> id_schet;
            if (dop_query.isValid())
            {
                do
                {
                    id_schet.append(dop_query.value(0).toInt());
                }while(dop_query.next());
            }
            dop_query.clear();

            int flag = 0;
            for(int i=0; i<id_schet.size(); i++)
            {
                dop_query.exec("SELECT id, summa FROM archiv_data WHERE id_schet="+QString::number(id_schet.at(i))+";");
                dop_query.first();
                if (dop_query.isValid())
                {
                    flag = i+1;
                    do
                    {
                        summa_oplat+=dop_query.value(1).toInt();
                    }while(dop_query.next());
                }
                dop_query.clear();
            }


            int row_i = ui->tableActive->rowCount();
            ui->tableActive->insertRow(row_i);

            ui->tableActive->setItem(row_i, 0, new QTableWidgetItem());
            ui->tableActive->item(row_i, 0)->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
            ui->tableActive->item(row_i, 0)->setCheckState(Qt::Unchecked);
            if (flag==1)
                ui->tableActive->item(row_i, 0)->setBackground(QColor(Qt::red));
            if (flag==2)
                ui->tableActive->item(row_i, 0)->setBackground(QColor(Qt::green));
            if (flag==3)
                ui->tableActive->item(row_i, 0)->setBackground(QColor(Qt::yellow));

            ui->tableActive->setItem(row_i, 1, new QTableWidgetItem(query.value(0).toString()));
            ui->tableActive->item(row_i, 1)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            if (flag==1)
                ui->tableActive->item(row_i, 1)->setBackground(QColor(Qt::red));
            if (flag==2)
                ui->tableActive->item(row_i, 1)->setBackground(QColor(Qt::green));
            if (flag==3)
                ui->tableActive->item(row_i, 1)->setBackground(QColor(Qt::yellow));

            ui->tableActive->setItem(row_i, 2, new QTableWidgetItem(query.value(1).toString()));
            ui->tableActive->item(row_i, 2)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            if (flag==1)
                ui->tableActive->item(row_i, 2)->setBackground(QColor(Qt::red));
            if (flag==2)
                ui->tableActive->item(row_i, 2)->setBackground(QColor(Qt::green));
            if (flag==3)
                ui->tableActive->item(row_i, 2)->setBackground(QColor(Qt::yellow));

            ui->tableActive->setItem(row_i, 3, new QTableWidgetItem(query.value(2).toString()));
            ui->tableActive->item(row_i, 3)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);
            if (flag==1)
                ui->tableActive->item(row_i, 3)->setBackground(QColor(Qt::red));
            if (flag==2)
                ui->tableActive->item(row_i, 3)->setBackground(QColor(Qt::green));
            if (flag==3)
                ui->tableActive->item(row_i, 3)->setBackground(QColor(Qt::yellow));

            if (for_bank==4 || for_bank==5)
            {
                ui->tableActive->setItem(row_i, 4, new QTableWidgetItem(QString::number(setting->value("min_zp").toInt())));
            }else
            {
                ui->tableActive->setItem(row_i, 4, new QTableWidgetItem(QString::number(list.at(i).summa.toInt()-summa_oplat)));
                if (flag==1)
                    ui->tableActive->item(row_i, 4)->setBackground(QColor(Qt::red));
                if (flag==2)
                    ui->tableActive->item(row_i, 4)->setBackground(QColor(Qt::green));
                if (flag==3)
                    ui->tableActive->item(row_i, 4)->setBackground(QColor(Qt::yellow));
            }

            if (for_bank==4 || for_bank==5)
                summa+=setting->value("min_zp").toInt();
            else
                summa+=list.at(i).summa.toInt()-summa_oplat;
            count_rec++;
        }

    }
    ui->tableActive->horizontalHeader()->resizeSections(QHeaderView::ResizeToContents);
    ui->label_count->setText("Всего записей: "+QString::number(count_rec_sel)+" из "+QString::number(count_rec));
    ui->label_summa->setText("Всего сумма: "+QString::number(summa_sel)+" из "+QString::number(summa));
}

void MainForm::on_action_export_triggered()
{
    QString str;
    switch(for_bank)
    {
        case 0:
        {
            QMessageBox::information(this, "Экспорт списка", "Не известно для какого банка делать экспорт!");
            return;
        }
        case 1:
        {
            str = "БеларусБанк.txt";
            break;
        }
        case 2:
        {
            str = "БелАгроПромБанк.txt";
            break;
        }
        case 3:
        {
            str = "ПриорБанк.txt";
            break;
        }
        case 4:
        {
            str = "МинЗПБеларусБанк.txt";
            break;
        }
        case 5:
        {
            str = "МинЗПБелАгроПромБанк.txt";
            break;
        }
    }
    QString filename = QFileDialog::getSaveFileName(this, "Экспорт списка", "out/"+str, "Список (*.txt)");
    if (filename=="")
        return;
    QFile f(filename);
    if (!f.open(QIODevice::WriteOnly))
    {
        QMessageBox::information(this, "Экспорт списка", "Не удаеться создать файл!");
        return;
    }
    QTextStream stream(&f);
    int count_record = 0;
    int summa = 0;
    switch(for_bank)
    {
        case 1:
        {
            QString query_string = "INSERT INTO archiv ([date], name, summa) "
                    "VALUES ('"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'БеларусБанк список на зачисление', 0);";
            //qDebug()<<query_string;
            QSqlQuery query(QSqlDatabase::database());
            if (!query.exec(query_string))
                qDebug()<<query.lastError().text();
            query_string = "SELECT TOP 1 id FROM archiv ORDER BY id DESC";
            if (!query.exec(query_string))
                qDebug()<<query.lastError().text();
            query.first();
            int id_archiv = query.value(0).toInt();
            qDebug()<<id_archiv;

            for (int i=0; i<ui->tableActive->rowCount(); i++)
            {
                if (ui->tableActive->item(i, 0)->checkState()==Qt::Checked)
                {
                    stream<<ui->tableActive->item(i, 2)->text()<<" "
                          <<ui->tableActive->item(i, 3)->text()<<" "
                          <<ui->tableActive->item(i, 4)->text()<<" "
                          <<ui->tableActive->item(i, 1)->text()
                          <<"\r\n";
                    count_record++;
                    summa+=ui->tableActive->item(i, 4)->text().toInt();

                    query_string = "SELECT id FROM schet WHERE kod='"+
                            ui->tableActive->item(i, 2)->text()+
                            "' AND schet='"+ui->tableActive->item(i, 3)->text()+"';";
                    //qDebug()<<query_string;
                    query.clear();
                    if (!query.exec(query_string))
                        qDebug()<<query.lastError().text();
                    query.first();
                    int id_schet = query.value(0).toInt();
                    query.clear();
                    query_string = "INSERT INTO archiv_data (id_archiv, id_schet, summa) "
                            "VALUES ("+QString::number(id_archiv)+", "+QString::number(id_schet)+", "+ui->tableActive->item(i, 4)->text()+");";
                    qDebug()<<query_string;
                    if (!query.exec(query_string))
                        qDebug()<<query.lastError().text();

                }
            }

            query.clear();
            query_string = "UPDATE archiv "
                    "SET summa="+QString::number(summa)+
                    " WHERE id="+QString::number(id_archiv)+";";
            qDebug()<<query_string;
            if (!query.exec(query_string))
                qDebug()<<query.lastError().text();

            break;
        }
        case 2:
        {
            QString query_string = "INSERT INTO archiv ([date], name, summa) "
                    "VALUES ('"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'БелАгроПромБанк список на зачисление', 0);";
            //qDebug()<<query_string;
            QSqlQuery query(QSqlDatabase::database());
            if (!query.exec(query_string))
                qDebug()<<query.lastError().text();
            query_string = "SELECT TOP 1 id FROM archiv ORDER BY id DESC";
            if (!query.exec(query_string))
                qDebug()<<query.lastError().text();
            query.first();
            int id_archiv = query.value(0).toInt();
            qDebug()<<id_archiv;

            for (int i=0; i<ui->tableActive->rowCount(); i++)
            {
                if (ui->tableActive->item(i, 0)->checkState()==Qt::Checked)
                {
                    stream<<ui->tableActive->item(i, 2)->text()<<","
                          <<ui->tableActive->item(i, 3)->text()<<","
                          <<ui->tableActive->item(i, 1)->text()<<","
                          <<ui->tableActive->item(i, 4)->text()
                          <<"\r\n";
                    count_record++;
                    summa+=ui->tableActive->item(i, 4)->text().toInt();

                    query_string = "SELECT id FROM schet WHERE kod='"+
                            ui->tableActive->item(i, 2)->text()+
                            "' AND schet='"+ui->tableActive->item(i, 3)->text()+"';";
                    //qDebug()<<query_string;
                    query.clear();
                    if (!query.exec(query_string))
                        qDebug()<<query.lastError().text();
                    query.first();
                    int id_schet = query.value(0).toInt();
                    query.clear();
                    query_string = "INSERT INTO archiv_data (id_archiv, id_schet, summa) "
                            "VALUES ("+QString::number(id_archiv)+", "+QString::number(id_schet)+", "+ui->tableActive->item(i, 4)->text()+");";
                    qDebug()<<query_string;
                    if (!query.exec(query_string))
                        qDebug()<<query.lastError().text();

                }
            }

            query.clear();
            query_string = "UPDATE archiv "
                    "SET summa="+QString::number(summa)+
                    " WHERE id="+QString::number(id_archiv)+";";
            qDebug()<<query_string;
            if (!query.exec(query_string))
                qDebug()<<query.lastError().text();
            break;
        }
        case 3:
        {
            QString query_string = "INSERT INTO archiv ([date], name, summa) "
                    "VALUES ('"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'ПриорБанк список на зачисление', 0);";
            //qDebug()<<query_string;
            QSqlQuery query(QSqlDatabase::database());
            if (!query.exec(query_string))
                qDebug()<<query.lastError().text();
            query_string = "SELECT TOP 1 id FROM archiv ORDER BY id DESC";
            if (!query.exec(query_string))
                qDebug()<<query.lastError().text();
            query.first();
            int id_archiv = query.value(0).toInt();
            qDebug()<<id_archiv;

            for (int i=0; i<ui->tableActive->rowCount(); i++)
            {
                if (ui->tableActive->item(i, 0)->checkState()==Qt::Checked)
                {
                    stream<<ui->tableActive->item(i, 2)->text()<<","
                          <<ui->tableActive->item(i, 3)->text()<<","
                          <<ui->tableActive->item(i, 1)->text()<<","
                          <<ui->tableActive->item(i, 4)->text()
                          <<"\r\n";
                    count_record++;
                    summa+=ui->tableActive->item(i, 4)->text().toInt();

                    query_string = "SELECT id FROM schet WHERE kod='"+
                            ui->tableActive->item(i, 2)->text()+
                            "' AND schet='"+ui->tableActive->item(i, 3)->text()+"';";
                    //qDebug()<<query_string;
                    query.clear();
                    if (!query.exec(query_string))
                        qDebug()<<query.lastError().text();
                    query.first();
                    int id_schet = query.value(0).toInt();
                    query.clear();
                    query_string = "INSERT INTO archiv_data (id_archiv, id_schet, summa) "
                            "VALUES ("+QString::number(id_archiv)+", "+QString::number(id_schet)+", "+ui->tableActive->item(i, 4)->text()+");";
                    qDebug()<<query_string;
                    if (!query.exec(query_string))
                        qDebug()<<query.lastError().text();

                }
            }

            query.clear();
            query_string = "UPDATE archiv "
                    "SET summa="+QString::number(summa)+
                    " WHERE id="+QString::number(id_archiv)+";";
            qDebug()<<query_string;
            if (!query.exec(query_string))
                qDebug()<<query.lastError().text();
            break;
        }
        case 4:
        {
            QString query_string = "INSERT INTO archiv ([date], name, summa) "
                    "VALUES ('"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'БеларусБанк список минимальная заработная плата', 0);";
            //qDebug()<<query_string;
            QSqlQuery query(QSqlDatabase::database());
            if (!query.exec(query_string))
                qDebug()<<query.lastError().text();
            query_string = "SELECT TOP 1 id FROM archiv ORDER BY id DESC";
            if (!query.exec(query_string))
                qDebug()<<query.lastError().text();
            query.first();
            int id_archiv = query.value(0).toInt();
            qDebug()<<id_archiv;

            for (int i=0; i<ui->tableActive->rowCount(); i++)
            {
                if (ui->tableActive->item(i, 0)->checkState()==Qt::Checked)
                {
                    stream<<ui->tableActive->item(i, 2)->text()<<" "
                          <<ui->tableActive->item(i, 3)->text()<<" "
                          <<ui->tableActive->item(i, 4)->text()<<" "
                          <<ui->tableActive->item(i, 1)->text()
                          <<"\r\n";
                    count_record++;
                    summa+=ui->tableActive->item(i, 4)->text().toInt();

                    query_string = "SELECT id FROM schet WHERE kod='"+
                            ui->tableActive->item(i, 2)->text()+
                            "' AND schet='"+ui->tableActive->item(i, 3)->text()+"';";
                    //qDebug()<<query_string;
                    query.clear();
                    if (!query.exec(query_string))
                        qDebug()<<query.lastError().text();
                    query.first();
                    int id_schet = query.value(0).toInt();
                    query.clear();
                    query_string = "INSERT INTO archiv_data (id_archiv, id_schet, summa) "
                            "VALUES ("+QString::number(id_archiv)+", "+QString::number(id_schet)+", "+ui->tableActive->item(i, 4)->text()+");";
                    qDebug()<<query_string;
                    if (!query.exec(query_string))
                        qDebug()<<query.lastError().text();

                }
            }

            query.clear();
            query_string = "UPDATE archiv "
                    "SET summa="+QString::number(summa)+
                    " WHERE id="+QString::number(id_archiv)+";";
            qDebug()<<query_string;
            if (!query.exec(query_string))
                qDebug()<<query.lastError().text();

            break;
        }
        case 5:
        {
            QString query_string = "INSERT INTO archiv ([date], name, summa) "
                    "VALUES ('"+QDate::currentDate().toString("dd.MM.yyyy")+"', 'БелАгроПромБанк список минимальная заработная плата', 0);";
            //qDebug()<<query_string;
            QSqlQuery query(QSqlDatabase::database());
            if (!query.exec(query_string))
                qDebug()<<query.lastError().text();
            query_string = "SELECT TOP 1 id FROM archiv ORDER BY id DESC";
            if (!query.exec(query_string))
                qDebug()<<query.lastError().text();
            query.first();
            int id_archiv = query.value(0).toInt();
            qDebug()<<id_archiv;

            for (int i=0; i<ui->tableActive->rowCount(); i++)
            {
                if (ui->tableActive->item(i, 0)->checkState()==Qt::Checked)
                {
                    stream<<ui->tableActive->item(i, 2)->text()<<","
                          <<ui->tableActive->item(i, 3)->text()<<","
                          <<ui->tableActive->item(i, 1)->text()<<","
                          <<ui->tableActive->item(i, 4)->text()
                          <<"\r\n";
                    count_record++;
                    summa+=ui->tableActive->item(i, 4)->text().toInt();

                    query_string = "SELECT id FROM schet WHERE kod='"+
                            ui->tableActive->item(i, 2)->text()+
                            "' AND schet='"+ui->tableActive->item(i, 3)->text()+"';";
                    //qDebug()<<query_string;
                    query.clear();
                    if (!query.exec(query_string))
                        qDebug()<<query.lastError().text();
                    query.first();
                    int id_schet = query.value(0).toInt();
                    query.clear();
                    query_string = "INSERT INTO archiv_data (id_archiv, id_schet, summa) "
                            "VALUES ("+QString::number(id_archiv)+", "+QString::number(id_schet)+", "+ui->tableActive->item(i, 4)->text()+");";
                    qDebug()<<query_string;
                    if (!query.exec(query_string))
                        qDebug()<<query.lastError().text();

                }
            }

            query.clear();
            query_string = "UPDATE archiv "
                    "SET summa="+QString::number(summa)+
                    " WHERE id="+QString::number(id_archiv)+";";
            qDebug()<<query_string;
            if (!query.exec(query_string))
                qDebug()<<query.lastError().text();
            break;
        }
    }

    QMessageBox::information(this, "Экспорт списка", "Список экспортирован\r\n"
                             "Количество записей: "+QString::number(count_record)+"\r\n"
                             "Общая сумма: "+QString::number(summa));
    f.close();
}

void MainForm::on_tableActive_clicked(const QModelIndex &index)
{
    count_rec = summa = 0;
    count_rec_sel = summa_sel = 0;
    for (int i=0; i<ui->tableActive->rowCount(); i++)
    {
        if (ui->tableActive->item(i, 0)->checkState()==Qt::Checked)
        {
            summa_sel+=ui->tableActive->item(i, 4)->text().toInt();
            count_rec_sel++;
        }
        count_rec++;
        summa+=ui->tableActive->item(i, 4)->text().toInt();
    }
    ui->label_count->setText("Всего записей: "+QString::number(count_rec_sel)+" из "+QString::number(count_rec));
    ui->label_summa->setText("Всего сумма: "+QString::number(summa_sel)+" из "+QString::number(summa));
}

void MainForm::on_action_triggered()
{
    AboutForm f;
    f.exec();
}

void MainForm::on_tabWidget_currentChanged(int index)
{
    switch(index)
    {
    case 0:
    {
        ui->action_add_record->setEnabled(true);
        ui->action_del_record->setEnabled(true);
        ui->action_import->setEnabled(true);
        ui->action_export->setEnabled(true);
        ui->action_new->setEnabled(true);

        ui->action_del_archiv->setEnabled(false);
        ui->action_del_all_archiv->setEnabled(false);

        break;
    }
    case 1:
    {
        ui->action_add_record->setEnabled(false);
        ui->action_del_record->setEnabled(false);
        ui->action_import->setEnabled(false);
        ui->action_export->setEnabled(false);
        ui->action_new->setEnabled(false);

        ui->action_del_archiv->setEnabled(true);
        ui->action_del_all_archiv->setEnabled(true);
        PrintArchiv();
        break;
    }
    }
}

void MainForm::on_tableArchiv_clicked(const QModelIndex &index)
{
    QString id ="";
    QTableWidgetItem* item = 0;
    item = ui->tableArchiv->item(index.row(), 0);
    if (item!=0)
    {
        id = item->text();
        if (id!="" && id.toInt()!=0)
            PrintArchivData(id);
    }
    //qDebug()<<"PrintArchivData() id="<<id;
}

void MainForm::on_action_del_archiv_triggered()
{
    QString id ="";
    QTableWidgetItem* item = 0;
    item = ui->tableArchiv->currentItem();
    if (item==0)
    {
        QMessageBox::about(this, "Удаление записи", "Необходимо выбрать запись для удаления!");
        return;
    }
    id = ui->tableArchiv->item(item->row(), 0)->text();
    if (id!="" && id.toInt()!=0)
    {
        if (QMessageBox::question(this, "Удаление записи", "Вы уверены что хотите удалить запись?")==QMessageBox::Yes)
        {
            QString query_string = "DELETE FROM archiv WHERE id="+id+";";
            QSqlQuery query(QSqlDatabase::database());
            if (!query.exec(query_string))
            {
                QMessageBox::warning(this, "Ошибка запроса", query.lastError().text());
                return;
            }
            PrintArchiv();
            ui->tableArchivData->setRowCount(0);
        }
    }
}

void MainForm::on_action_del_record_triggered()
{
    QTableWidgetItem* item = 0;
    item = ui->tableActive->currentItem();
    if (item==0)
    {
        QMessageBox::about(this, "Удаление записи", "Необходимо выбрать запись для удаления!");
        return;
    }
    if (QMessageBox::question(this, "Удаление записи", "Вы уверены что ходите удалить запись?")==QMessageBox::Yes)
    {
        ui->tableActive->removeRow(item->row());
        QModelIndex index;
        this->on_tableActive_clicked(index);
    }
}

void MainForm::on_action_add_record_triggered()
{
    insert_form->Init();
    insert_form->exec();
}

void MainForm::on_add_record(QString id_persona, QString id_bank, QString summa)
{
    QString query_string = "SELECT persona.fio, schet.kod, schet.schet "
            "FROM persona INNER JOIN schet ON persona.id = schet.id_persona "
            "WHERE schet.id_bank="+id_bank+" AND schet.id_persona="+id_persona+";";
    QSqlQuery query(QSqlDatabase::database());
    if (!query.exec(query_string))
    {
        QMessageBox::warning(this, "Ошибка запроса", query.lastError().text());
        return;
    }
    if (query.isSelect())
        query.first();
    if (query.isValid())
    {
        int row = ui->tableActive->rowCount();
        ui->tableActive->insertRow(row);

        ui->tableActive->setItem(row, 0, new QTableWidgetItem());
        ui->tableActive->item(row, 0)->setFlags(Qt::ItemIsUserCheckable|Qt::ItemIsEnabled);
        ui->tableActive->item(row, 0)->setCheckState(Qt::Unchecked);

        ui->tableActive->setItem(row, 1, new QTableWidgetItem(query.value(0).toString()));
        ui->tableActive->item(row, 1)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

        ui->tableActive->setItem(row, 2, new QTableWidgetItem(query.value(1).toString()));
        ui->tableActive->item(row, 2)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

        ui->tableActive->setItem(row, 3, new QTableWidgetItem(query.value(2).toString()));
        ui->tableActive->item(row, 3)->setFlags(Qt::ItemIsSelectable|Qt::ItemIsEnabled);

        ui->tableActive->setItem(row, 4, new QTableWidgetItem(summa));
    }
    QModelIndex index;
    this->on_tableActive_clicked(index);
}

void MainForm::on_action_new_triggered()
{
    ui->label_bank->setText("Формирование списка для:");
    SelectImportForm f;
    int bank = f.exec();
    //qDebug()<<for_bank;
    if (!bank)
        return;
    for_bank = bank;

    switch(for_bank)
    {
        case 1:
        {
            ui->label_bank->setText("Формирование списка для: БеларусБанк");
            break;
        }
        case 2:
        {
            ui->label_bank->setText("Формирование списка для: БелАгроПромБанк");
            break;
        }
    }

    count_rec = count_rec_sel = summa = summa_sel = 0;
    ui->tableActive->setRowCount(0);
}

void MainForm::on_action_minimal_zp_triggered()
{
    MinimalZPForm f;
    f.min_zp = setting->value("min_zp").toInt();
    int flag = f.exec();
    if (flag!=-1)
        setting->setValue("min_zp", f.min_zp);
}

void MainForm::on_action_del_all_archiv_triggered()
{

    if (QMessageBox::question(this, "Удаление всех записей", "Вы уверены что хотите удалить все записи?")==QMessageBox::Yes)
    {
        QString query_string = "DELETE FROM archiv;";
        QSqlQuery query(QSqlDatabase::database());
        if (!query.exec(query_string))
        {
            QMessageBox::warning(this, "Ошибка запроса", query.lastError().text());
            return;
        }
        PrintArchiv();
        ui->tableArchivData->setRowCount(0);
    }
}
