//
// Created by jayce on 09/11/2016.
//

#ifndef C_PLUS_PLUS_PRACTICE_EDGE_CLUSTER_EDGE_H
#define C_PLUS_PLUS_PRACTICE_EDGE_CLUSTER_EDGE_H
#include <iostream>
using namespace std;
/*
 * 边簇图中的边，对应着原图的顶点
 * 边图中的两个顶点存在边，对应着原图两天边存在共同顶点
 * 作为统计使用，一般用不上
 */
class Node;
class EC_Node;
class EC_Edge{
public:
    EC_Edge(long long id,Node* p_node, EC_Node* node1,EC_Node *node2)
    {
        this->id = id; // id自增长，从1开始，与原图度有关
        this->src_node = p_node;
        this->ec_node1 = node1;
        this->ec_node2 = node2;
    }
    long long getId() const {
        return id;
    }

    Node *getSrc_node() const {
        return src_node;
    }

    EC_Node *getEc_node1() const {
        return ec_node1;
    }

    EC_Node *getEc_node2() const {
        return ec_node2;
    }
private:
    long long id;
    Node* src_node;
    EC_Node* ec_node1;
    EC_Node* ec_node2;
};
#endif //C_PLUS_PLUS_PRACTICE_EDGE_CLUSTER_EDGE_H
