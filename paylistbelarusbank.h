#ifndef PAYLISTBELARUSBANK_H
#define PAYLISTBELARUSBANK_H

#include <QDialog>

namespace Ui {
class PayListBelarusbank;
}

class PayListBelarusbank : public QDialog
{
    Q_OBJECT

public:
    explicit PayListBelarusbank(QWidget *parent = 0);
    ~PayListBelarusbank();
    void init();
private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::PayListBelarusbank *ui;
};

#endif // PAYLISTBELARUSBANK_H
