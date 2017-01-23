//
//  widget_painter.cpp
//  qtgo
//
//  Created by jayce on 23/01/2017.
//
//

#include "widget_painter.h"
#include <iostream>
#include <Qpainter>
using namespace std;

Widget_Painter::Widget_Painter(QWidget *parent)
    :QWidget(parent)
{
    p_pixmap = nullptr;
}

Widget_Painter::~Widget_Painter()
{
    if(p_pixmap!=nullptr){
        delete p_pixmap;
    }
}

void Widget_Painter::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    if(p_pixmap!=nullptr){
        QPainter painter(this);
        painter.drawPixmap(0,0,100,100,*p_pixmap);
    }
    cout << "wp painter---" << endl;
}
