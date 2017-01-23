#include "ntiss.h"
#include "ui_ntiss.h"
#include <string>
using namespace std;

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
    QPlainTextEdit *plainTextEdit = ui->plainTextEdit;
    QString tbx_str = plainTextEdit->toPlainText();
    QMessageBox::about(NULL, "About", tbx_str);
}

