//
// Created by jayce on 09/11/2016.
//

#ifndef C_PLUS_PLUS_PRACTICE_EDGE_CLUSTER_NODE_H
#define C_PLUS_PLUS_PRACTICE_EDGE_CLUSTER_NODE_H
#include "poi.h"
#include "lbs_user.h"

#include <vector>
using namespace std;

/*
 * 边簇图的顶点对应原图的边
 * 作为路网遍历使用
 */
class EC_Edge;
class Edge;
class EC_Node{
public:
    /*
     * 边图中的 顶点 id = 原图 边 id
     */
    EC_Node(long long id, Edge *src_edge,double x,double y,const vector<LBS_User*> &users,const vector<Poi*> &pois)
    {
        this->ec_id = id;
		this->src_edge = src_edge;
		this->x = x;
		this->y = y;
		this->users = users;
		this->pois = pois;
    }
    void add_adj_node(EC_Node* node,EC_Edge* edge)
    {
        /*
         * 两个顶点间多条边的情况时
         * 这些原图边对应的边簇图顶点添加临近顶点时·允许重复·，对应这原图这些边通过两个顶点发生联系，而不是一个
         */
        adj_nodes.push_back(node);
        related_edges.push_back(edge);
    }
    long long get_id()
    {
        return ec_id;
    }
	Edge*& get_src_edge()
	{
		return src_edge;
	}
	double get_x()
	{
		return x;
	}
	double get_y()
	{
		return y;
	}
    vector<EC_Node*>& get_adj_nodes()
    {
        return adj_nodes;
    }
	const vector<Poi*>& get_pois()
	{
		return pois;
	}
	const vector<LBS_User*>& get_users()
	{
		return users;
	}
private:
    long long ec_id;
	double x, y;
    vector<EC_Node*> adj_nodes;
    vector<EC_Edge*> related_edges; //记录边图中，一个顶点产生相邻接顶点对应边图的边,一般用不上
	vector<LBS_User*> users;
	vector<Poi*> pois;
	Edge *src_edge;
};
#endif //C_PLUS_PLUS_PRACTICE_EDGE_CLUSTER_NODE_H
