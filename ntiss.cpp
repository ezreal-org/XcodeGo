#include "ntiss.h"
#include "ui_ntiss.h"
#include <string>
#include <iostream>
#include <QPainter>
#include <QPixmap>
using namespace std;

ntiss::ntiss(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::ntiss)
{
    p_pixmap = nullptr;
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
    if(p_pixmap==nullptr){
        p_pixmap = new QPixmap(100,100);
        p_pixmap->fill(Qt::transparent);
        QPainter painter(p_pixmap);
        painter.setPen(QPen(Qt::red,2));
        painter.drawLine(QPointF(0, 0), QPointF(50, 50 / 2));
        ui->label->setPixmap(*p_pixmap);  //这个会引起重绘
    }
}

void ntiss::paintEvent(QPaintEvent *event)
{
    if(p_pixmap!=nullptr){
        QPainter painter(this);
        painter.drawPixmap(0,0,100,100,*p_pixmap);
    }
    cout << "fuck paint" << endl;
}
