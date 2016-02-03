#ifndef MINIMALZPFORM_H
#define MINIMALZPFORM_H

#include <QDialog>
#include <QMessageBox>

namespace Ui {
class MinimalZPForm;
}

class MinimalZPForm : public QDialog
{
    Q_OBJECT

public:
    explicit MinimalZPForm(QWidget *parent = 0);
    ~MinimalZPForm();
    void showEvent(QShowEvent *event);
    int min_zp;

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::MinimalZPForm *ui;
};

#endif // MINIMALZPFORM_H
