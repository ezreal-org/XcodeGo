#include "ntiss.h"
#include "ui_ntiss.h"
#include <iostream>
#include <string>
#include <iostream>
#include <QPainter>
#include <QPixmap>
using namespace std;

ntiss::ntiss(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::ntiss)
{
    ui->setupUi(this); //设置ui,qt帮注册 信号-槽
    ui->comboBox->currentIndexChanged(0); //触发信号、让默认选择一个地图

    this->p_graph = nullptr;
}

ntiss::~ntiss()
{
    delete ui;
}

void ntiss::on_pushButton_clicked()
{
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

void ntiss::on_pushButton_6_clicked()
{
    cout << "run" << endl;
}

void ntiss::on_comboBox_currentIndexChanged(int index)
{
    ifstream if_nodes, if_edges;
    string config_path = "";
    string workspace_path = "/Users/jayce/Documents/workspace/cpp/qtgo/";
    if (index == 0) {
        if_nodes.open(workspace_path + "graph/map_data/oldenburgGen_node.txt");
        if_edges.open(workspace_path +"graph/map_data/oldenburgGen_edge.txt");
        config_path = workspace_path +"graph/map_data/graph_config_oldenburgGen.txt";
    }
    else if (index == 1) {
        if_nodes.open(workspace_path +"graph/map_data/sanfrancisco_node.txt");
        if_edges.open(workspace_path +"graph/map_data/sanfrancisco_edge.txt");
        config_path = workspace_path +"graph/map_data/graph_config_sanfrancisco.txt";
    }
    this->ui->btn_draw_src->setText("等待");
    this->ui->btn_draw_ec->setText("等待");
    this->ui->btn_draw_src->setEnabled(false);
    this->ui->btn_draw_ec->setEnabled(false);
    this->repaint();
    qApp->processEvents();
    if (this->p_graph != nullptr) {
        delete(this->p_graph);
    }
    EC_Graph *p_graph = new EC_Graph(if_nodes, if_edges,config_path);
    this->p_graph = p_graph;
    this->ui->btn_draw_src->setText("绘制原图");
    this->ui->btn_draw_ec->setText("绘边簇图");
    this->ui->btn_draw_src->setEnabled(true);
    this->ui->btn_draw_ec->setEnabled(true);
    if_nodes.close();
    if_edges.close();

    cout << p_graph->p_graph->get_pois().size() << endl;
}

