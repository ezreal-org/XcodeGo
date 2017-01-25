//
//  widget_painter.h
//  qtgo
//
//  Created by jayce on 23/01/2017.
//
//

#ifndef widget_painter_h
#define widget_painter_h

#include <QFrame>
#include <QWidget>
#include "graph/draw_util.h"

class Frame_Painter :public QFrame
{
    Q_OBJECT
public:
    explicit Frame_Painter(QWidget *parent);
    ~Frame_Painter();
    
private slots:
    void paintEvent(QPaintEvent*);
public:
    QPixmap *p_pixmap;
    Draw_Util *p_du;
};

#endif /* widget_painter_h */
