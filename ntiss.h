#ifndef NTISS_H
#define NTISS_H

#include <QMainWindow>
#include <QMessageBox>
#include "graph/edge_cluster_graph.h"

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
    void on_pushButton_6_clicked();
    void on_comboBox_currentIndexChanged(int index);
private:
    Ui::ntiss *ui;
    EC_Graph *p_graph;
};

#endif // NTISS_H
