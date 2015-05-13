#ifndef BANKFORM_H
#define BANKFORM_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QMessageBox>
#include <QSqlError>

namespace Ui {
class BankForm;
}

class BankForm : public QMainWindow
{
    Q_OBJECT

public:
    explicit BankForm(QWidget *parent = 0);
    ~BankForm();
    void Init();

private slots:
    void on_action_exit_triggered();

    void on_action_add_triggered();

    void on_action_del_triggered();

    void on_action_save_triggered();

private:
    Ui::BankForm *ui;
    QSqlTableModel* bank_model;
};

#endif // BANKFORM_H
