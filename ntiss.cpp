#include "ntiss.h"
#include "ui_ntiss.h"

ntiss::ntiss(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::ntiss)
{
    ui->setupUi(this);
}

ntiss::~ntiss()
{
    delete ui;
}

void ntiss::on_pushButton_clicked()
{
    ui->pushButton->setText("hello tiankai");
    QMessageBox::about(NULL, "About", "good night");
}
