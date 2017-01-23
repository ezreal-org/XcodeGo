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
    void paintEvent(QPaintEvent *event);

private:
    Ui::ntiss *ui;
    QPixmap *p_pixmap;
};

#endif // NTISS_H
