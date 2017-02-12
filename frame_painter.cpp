//
//  widget_painter.cpp
//  qtgo
//
//  Created by jayce on 23/01/2017.
//
//

#include "frame_painter.h"
#include <iostream>
#include <QPainter>
using namespace std;

Frame_Painter::Frame_Painter(QWidget *parent)
    :QFrame(parent)
{
    p_pixmap = nullptr;
}

Frame_Painter::~Frame_Painter()
{
    if(p_pixmap!=nullptr){
        delete p_pixmap;
    }
}

void Frame_Painter::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e);
    if(p_pixmap!=nullptr){
        QPainter painter(this);
        painter.drawPixmap(0,0,p_du->get_paint_sizex(),p_du->get_paint_sizey(),*p_pixmap);
    }
    cout << "wp painter---" << endl;
}
