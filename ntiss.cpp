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
    QPixmap *&p_pixmap = ui->widget->p_pixmap;
    if(p_pixmap==nullptr){
        p_pixmap = new QPixmap(450,450);
        p_pixmap->fill(Qt::transparent);
        QPainter painter(p_pixmap);  //绘制图形到 pixmap
        painter.setPen(QPen(Qt::red,2));

        for(int i=0;i<100;i++){
            for(int j=0;j<100;j++)
                painter.drawLine(QPointF(i, j), QPointF(300, 300 / 2));
        }
       // ui->label->setPixmap(*p_pixmap);  //这个会引起重绘--..用Painter执行的操作才`不会`引起重绘
       ui->widget->update(); //button点击本身也会引起重绘，不加也没事
    }
}
