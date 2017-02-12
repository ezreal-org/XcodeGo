#include "ntiss.h"
#include "ui_ntiss.h"
#include <iostream>
#include <string>
#include <iostream>
#include <QPainter>
#include <QPixmap>
#include "graph/lpp_algorithm/cfso.h"
#include "graph/lpp_algorithm/plpca.h"
#include "graph/lpp_algorithm/replay_attack.h"
#include "graph/lpp_algorithm/sa.h"
using namespace std;

ntiss::ntiss(QWidget *parent) :
QMainWindow(parent),
ui(new Ui::ntiss)
{
    ui->setupUi(this); //设置ui,qt帮注册 信号-槽
    init(); //控件对象还在内存中没有显示前--即paint 之前不应该去操作它
}

void ntiss::init()
{
    this->p_graph = nullptr;
}

ntiss::~ntiss()
{
    delete ui;
}

// paintEvent 中应该只完成一些跟绘图无关的计算或数据准备
void ntiss::paintEvent(QPaintEvent *event)
{
    static bool is_first_call=true;
    if(is_first_call){
        //注意防止出现递归绘制
        is_first_call = false;
        ifstream if_nodes, if_edges;
        string config_path = "";
        string workspace_path = "/Users/jayce/Documents/workspace/cpp/qtgo/";
        if_nodes.open(workspace_path + "graph/map_data/oldenburgGen_node.txt");
        if_edges.open(workspace_path +"graph/map_data/oldenburgGen_edge.txt");
        config_path = workspace_path +"graph/map_data/graph_config_oldenburgGen.txt";
        EC_Graph *p_graph = new EC_Graph(if_nodes, if_edges,config_path);
        this->p_graph = p_graph;
        if_nodes.close();
        if_edges.close();
        this->ui->btn_zoom_in->setEnabled(false);
        on_btn_draw_src_clicked();
    }
}

//这个函数只包含pixmal数据设置
void ntiss::on_pushButton_clicked() //
{
    //Lppa_sa *p_sa = new Lppa_sa(this->p_graph->p_graph);
    //Lppa_plpca *p_sa = new Lppa_plpca(this->p_graph);
    //Lppa_ecsa_e *p_sa = new Lppa_ecsa_e(this->p_graph);
    
    //p_sa->lpp();
    //delete p_sa;
    
    Replay_attack *ra = new Replay_attack(this->p_graph);
    delete ra;
}

void ntiss::compute_ec_graph()
{
    QPen red_pen(Qt::red,1.0f);
    QPixmap *&p_pixmap = ui->widget->p_pixmap;
    p_pixmap = new QPixmap(p_du->get_paint_sizex(),p_du->get_paint_sizey());
    p_pixmap->fill(Qt::transparent);
    QPainter painter(p_pixmap);  //绘制图形到 pixmap
    painter.setPen(red_pen);
    int edge_cnt = p_graph->get_edges().size();
    cout << edge_cnt << endl;
    vector<EC_Edge*> edges = p_graph->get_edges();
    double x1, x2, y1, y2;
    EC_Node *p_node1, *p_node2;
    for (int i = 0; i < edge_cnt; i++)
    {
        p_node1 = edges[i]->getEc_node1();
        p_node2 = edges[i]->getEc_node2();
        x1 = p_node1->get_x();
        y1 = p_node1->get_y();
        x2 = p_node2->get_x();
        y2 = p_node2->get_y();
        p_du->convert_to_draw_pos(x1, y1);
        p_du->convert_to_draw_pos(x2, y2);
        if ((x1<0 && x2<0) || (x1>p_du->get_paint_sizex() && x2>p_du->get_paint_sizex())) continue;
        if ((y1<0 && y2<0) || (y1>p_du->get_paint_sizey() && y2>p_du->get_paint_sizey())) continue;
        //if(edges[i]->getEdgeClass()<5)
        //mg->DrawLine(pen_list[edges[i]->getEdgeClass()], (float)x1, (float)y1, (float)x2, (float)y2);
        //else
        painter.drawLine(x1, y1, x2, y2);
    }
    ui->widget->update();
}

void ntiss::compute_src_graph()
{
    QPixmap *&p_pixmap = ui->widget->p_pixmap;
    p_pixmap = new QPixmap(p_du->get_paint_sizex(),p_du->get_paint_sizey());
    p_pixmap->fill(Qt::transparent);
    
    QPen red_pen = QPen(Qt::red,2.0f);
    QPen yellow_pen = QPen(Qt::yellow,1.8f);
    QPen blue_pen = QPen(Qt::blue,1.5f);
    QPen green_pen = QPen(Qt::green,1.0f);
    QPen dark_gray_pen = QPen(Qt::darkGray,0.5f);
    QPen gray_pen = QPen(Qt::gray,0.5f);
    QPen light_gray_pen = QPen(Qt::lightGray,0.5f);
    
    QPainter painter(p_pixmap);  //绘制图形到 pixmap
    QVector<QPen> v_p;
    v_p.append(red_pen);
    v_p.append(yellow_pen);
    v_p.append(blue_pen);
    v_p.append(green_pen);
    v_p.append(dark_gray_pen);
    v_p.append(gray_pen);
    v_p.append(light_gray_pen);
    
    
    int edge_cnt = p_graph->p_graph->getEdges().size();
    bool need_scaling = false;
    vector<Edge*> edges = p_graph->p_graph->getEdges();
    double x1, x2, y1, y2;
    Node *p_node1, *p_node2;
    //    bool need_scaling = this->checkBox1->Checked;
    for (int i = 0; i < edge_cnt; i++) {
        p_node1 = edges[i]->getNode1();
        p_node2 = edges[i]->getNode2();
        x1 = p_node1->getX();
        y1 = p_node1->getY();
        x2 = p_node2->getX();
        y2 = p_node2->getY();
        p_du->convert_to_draw_pos(x1, y1);
        p_du->convert_to_draw_pos(x2, y2);
        if ((x1<0 && x2<0) || (x1>p_du->get_paint_sizex() && x2>p_du->get_paint_sizex())) continue;
        if ((y1<0 && y2<0) || (y1>p_du->get_paint_sizey() && y2>p_du->get_paint_sizey())) continue;
        if ((edges[i]->getEdgeClass() < p_du->get_zoom() + 1 || !need_scaling)) {
            painter.setPen(v_p[edges[i]->getEdgeClass()]);
            painter.drawLine(x1, y1, x2, y2); //画边
            if (x1 == x2) continue;
            //ªÊ÷∆±ﬂ…œµƒ–À»§µ„
            int poi_size = edges[i]->get_pois().size();
            for (int j = 1; j <= poi_size; j++) {
                float x, y;
                x = x1 + ((float)j / (poi_size + 1)) * (x2-x1);
                y = y1 + (x-x1)*(y2 - y1) / (x2 - x1);
                if (edges[i]->get_pois()[j - 1]->get_type()==Semantic_type::hospital) {
                    //mg->DrawRectangle(red_pen, x, y, 5.0, 5.0);
                    painter.setPen(red_pen);
                    painter.drawRect(QRectF(x,y,3,3));
                }
                else{
                    painter.setPen(green_pen);
                    painter.drawRect(QRectF(x,y,3,3));
                }
                
            }
            // ªÊ÷∆±ﬂ…œµƒlbs”√ªß
            int user_cnt = edges[i]->get_users().size();
            for (int j = 1; j <= user_cnt; j++) {
                float x, y;
                x = x1 + ((float)j / (user_cnt + 2)) * (x2 - x1);
                y = y1 + (x - x1)*(y2 - y1) / (x2 - x1);
                painter.setPen(blue_pen);
                painter.drawEllipse(QRectF(x,y,1.5,1.5));
            }
        }
    }
    
    ui->widget->update(); //button点击本身也会引起重绘，不加也没事
}

void ntiss::on_comboBox_currentIndexChanged(int index)
{
    ifstream if_nodes, if_edges;
    string config_path = "";
    string workspace_path = "/Users/jayce/Documents/workspace/cpp/qtgo/";
    if (index == 0) {
        draw_index = 0;
        if_nodes.open(workspace_path + "graph/map_data/oldenburgGen_node.txt");
        if_edges.open(workspace_path +"graph/map_data/oldenburgGen_edge.txt");
        config_path = workspace_path +"graph/map_data/graph_config_oldenburgGen.txt";
    }
    else if (index == 1) {
        draw_index = 1;
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
    this->ui->btn_zoom_in->setEnabled(false);
    this->ui->btn_zoom_out->setEnabled(true);
    on_btn_draw_src_clicked();
}


void ntiss::on_btn_draw_src_clicked()
{
    draw_index= 0;
    double x_min = p_graph->p_graph->get_xmin();
    double x_max = p_graph->p_graph->get_xmax();
    double y_min = p_graph->p_graph->get_ymin();
    double y_max = p_graph->p_graph->get_ymax();
    p_du = new Draw_Util(x_min, x_max, y_min, y_max, this->ui->widget->width(), this->ui->widget->height(), 20);
    this->ui->widget->p_du = p_du;
    compute_src_graph();
    this->ui->btn_zoom_in->setEnabled(false);
    this->ui->btn_zoom_out->setEnabled(true);
}

void ntiss::on_btn_draw_ec_clicked()
{
    draw_index = 1;
    double x_min = p_graph->p_graph->get_xmin();
    double x_max = p_graph->p_graph->get_xmax();
    double y_min = p_graph->p_graph->get_ymin();
    double y_max = p_graph->p_graph->get_ymax();
    p_du = new Draw_Util(x_min, x_max, y_min, y_max, this->ui->widget->width(), this->ui->widget->height(), 20);
    this->ui->widget->p_du = p_du;
    compute_ec_graph();
    this->ui->btn_zoom_in->setEnabled(false);
    this->ui->btn_zoom_out->setEnabled(true);
}


void ntiss::on_btn_zoom_out_clicked()
{
    p_du->zoom_plus();
    if (p_du->get_zoom() > 6) this->ui->btn_zoom_out->setEnabled(false);
    this->ui->btn_zoom_in->setEnabled(true);
    if(draw_index==0){
        compute_src_graph();
    }else{
        compute_ec_graph();
    }
}

void ntiss::on_btn_zoom_in_clicked()
{
    p_du->zoom_sub();
    if (p_du->get_zoom() < 2) this->ui->btn_zoom_in->setEnabled(false);
    this->ui->btn_zoom_out->setEnabled(true);
    if(draw_index==0){
        compute_src_graph();
    }else{
        compute_ec_graph();
    }
    
}

void ntiss::on_pushButton_6_clicked() //up
{
    p_du->top_move();
    if(draw_index==0){
        compute_src_graph();
    }else{
        compute_ec_graph();
    }
}

void ntiss::on_pushButton_2_clicked() //down
{
    p_du->buttom_move();
    if(draw_index==0){
        compute_src_graph();
    }else{
        compute_ec_graph();
    }

}

void ntiss::on_pushButton_7_clicked() //left
{
    p_du->left_move();
    if(draw_index==0){
        compute_src_graph();
    }else{
        compute_ec_graph();
    }
}

void ntiss::on_pushButton_3_clicked() //right
{
    p_du->right_move();
    if(draw_index==0){
        compute_src_graph();
    }else{
        compute_ec_graph();
    }

}
