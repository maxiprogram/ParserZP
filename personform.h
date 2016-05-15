#ifndef PERSONFORM_H
#define PERSONFORM_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include <QMessageBox>
#include <QSqlError>
#include <QSqlRecord>
#include <QSqlRelationalDelegate>

namespace Ui {
class PersonForm;
}

class PersonForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit PersonForm(QWidget *parent = 0);
    ~PersonForm();
    void Init();

private slots:
    void on_tablePersona_changed(QModelIndex index);

    void on_action_exit_triggered();

    void on_action_add_person_triggered();

    void on_action_del_person_triggered();

    void on_action_save_person_triggered();

    void on_action_add_schet_triggered();

    void on_action_del_schet_triggered();

    void on_action_save_schet_triggered();

    void on_lineEdit_textChanged(const QString &arg1);

private:
    Ui::PersonForm *ui;
    QSqlTableModel* persona_model;
    QSqlRelationalTableModel* schet_model;
};

#endif // PERSONFORM_H
