#ifndef MAINFORM_H
#define MAINFORM_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QFileDialog>
#include <QSqlQuery>
#include <QDate>
#include <QTableWidgetItem>
#include <QSettings>

#include "bankform.h"
#include "personform.h"
#include "selectimportform.h"
#include "insertactiveform.h"
#include "minimalzpform.h"
#include "aboutform.h"
#include "parser.h"
#include "paylistbelarusbank.h"

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

    void on_action_minimal_zp_triggered();

    void on_action_del_all_archiv_triggered();

    void on_lineEdit_search_textChanged(const QString &arg1);

    void on_tableActive_cellChanged(int row, int column);

private:
    Ui::MainForm *ui;
    BankForm* bank_form;
    PersonForm* persona_form;
    InsertActiveForm* insert_form;
    PayListBelarusbank *payListBelarusbank;
    Parser* parser;
    int for_bank;
    int count_rec, count_rec_sel;
    double summa, summa_sel;
    QSettings* setting;
};

#endif // MAINFORM_H
