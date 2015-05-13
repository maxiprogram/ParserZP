#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QFileDialog>
#include <QSqlQuery>
#include <QDate>
#include <QTableWidgetItem>

#include "bankform.h"
#include "personform.h"
#include "selectimportform.h"
#include "insertactiveform.h"
#include "aboutform.h"
#include "parser.h"

namespace Ui {
class MainForm;
}

class MainForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();
    void PrintArchiv();
    void PrintArchivData(QString id);
    void Clear(QTableWidget* table);

private slots:
    void on_add_record(QString id_persona, QString id_bank, QString summa);

    void on_header_clicked(int index, Qt::SortOrder asc);

    void on_action_bank_triggered();

    void on_action_persona_triggered();

    void on_action_import_triggered();

    void on_action_export_triggered();

    void on_tableActive_clicked(const QModelIndex &index);

    void on_action_triggered();

    void on_tabWidget_currentChanged(int index);

    void on_tableArchiv_clicked(const QModelIndex &index);

    void on_action_del_archiv_triggered();

    void on_action_del_record_triggered();

    void on_action_add_record_triggered();

    void on_action_new_triggered();

private:
    Ui::MainForm *ui;
    BankForm* bank_form;
    PersonForm* persona_form;
    InsertActiveForm* insert_form;
    Parser* parser;
    int for_bank;
    int count_rec, count_rec_sel;
    int summa, summa_sel;
};

#endif // MAINFORM_H
