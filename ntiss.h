#ifndef NTISS_H
#define NTISS_H

#include <QMainWindow>
#include <QMessageBox>

namespace Ui {
class ntiss;
}

class ntiss : public QMainWindow
{
    Q_OBJECT

public:
    explicit ntiss(QWidget *parent = 0);
    ~ntiss();

private slots:
    void on_pushButton_clicked();

private:
    Ui::ntiss *ui;
};

#endif // NTISS_H
