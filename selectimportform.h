#ifndef SELECTIMPORTFORM_H
#define SELECTIMPORTFORM_H

#include <QDialog>

namespace Ui {
class SelectImportForm;
}

class SelectImportForm : public QDialog
{
    Q_OBJECT

public:
    explicit SelectImportForm(QWidget *parent = 0);
    ~SelectImportForm();

private slots:
    void on_pushButton_2_clicked();

    void on_pushButton_clicked();

private:
    Ui::SelectImportForm *ui;
};

#endif // SELECTIMPORTFORM_H
