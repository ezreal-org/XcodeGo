#ifndef NTISS_H
#define NTISS_H

#include <QMainWindow>
#include <QMessageBox>
#include "graph/edge_cluster_graph.h"
#include "graph/draw_util.h"

namespace Ui {
class ntiss;
}

class ntiss : public QMainWindow
{
    Q_OBJECT

public:
    explicit ntiss(QWidget *parent = 0);
    void init();
    ~ntiss();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_6_clicked();
    void on_comboBox_currentIndexChanged(int index);
    void paintEvent(QPaintEvent *event);
    void on_btn_draw_src_clicked();

    void on_btn_draw_ec_clicked();
    void compute_src_graph();
    void compute_ec_graph();
    void on_btn_zoom_out_clicked();
    void on_btn_zoom_in_clicked();

    void on_pushButton_2_clicked();

    void on_pushButton_7_clicked();

    void on_pushButton_3_clicked();

public:
    Ui::ntiss *ui;
    EC_Graph *p_graph;
    Draw_Util *p_du;
    int draw_index;
};

#endif // NTISS_H
