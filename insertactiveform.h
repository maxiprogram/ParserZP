#ifndef INSERTACTIVEFORM_H
#define INSERTACTIVEFORM_H

#include <QDialog>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>

namespace Ui {
class InsertActiveForm;
}

class InsertActiveForm : public QDialog
{
    Q_OBJECT

public:
    explicit InsertActiveForm(QWidget *parent = 0);
    void Init();
    ~InsertActiveForm();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

    void on_lineEdit_search_textChanged(const QString &arg1);

signals:
    void ADD_RECORD(QString id_persona, QString id_bank, QString summa);

private:
    Ui::InsertActiveForm *ui;
};

#endif // INSERTACTIVEFORM_H
