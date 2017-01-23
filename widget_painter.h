//
//  widget_painter.h
//  qtgo
//
//  Created by jayce on 23/01/2017.
//
//

#ifndef widget_painter_h
#define widget_painter_h

#include <QWidget>

class Widget_Painter :public QWidget
{
    Q_OBJECT
public:
    explicit Widget_Painter(QWidget *parent);
    ~Widget_Painter();
    
private slots:
    void paintEvent(QPaintEvent*);
public:
    QPixmap *p_pixmap;
};

#endif /* widget_painter_h */
