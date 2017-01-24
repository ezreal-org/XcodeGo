//
// Created by jayce on 09/11/2016.
//

#ifndef C_PLUS_PLUS_PRACTICE_EDGE_CLUSTER_GRAPH_H
#define C_PLUS_PLUS_PRACTICE_EDGE_CLUSTER_GRAPH_H

#include "edge_cluster_edge.h"
#include "edge_cluster_node.h"
#include "graph.h"

class EC_Graph{
public:
    EC_Graph(ifstream &f_nodes,ifstream &f_edges,string config_file)
    {
        cnt_of_ec_edges = 0; // 用于自增表示边图中的各边
        p_graph = new Graph(f_nodes,f_edges,config_file);
        //为每条边生成边图顶点
        cout << p_graph->getEdges().size() << " src_graph edges !!" << endl;
		double ec_node_x, ec_node_y;
		Node *p_node1, *p_node2;
		vector<Edge*> src_edges = p_graph->getEdges();
		size_t src_edge_size = src_edges.size();
		int node_id = 0;
        for(int i=0;i<src_edge_size;i++){
			Edge *p = src_edges[i];
			p_node1 = p->getNode1();
			p_node2 = p->getNode2();
			ec_node_x = (p_node1->getX() + p_node2->getX()) / 2;
			ec_node_y = (p_node1->getY() + p_node2->getY()) / 2;
			//边图中顶点的id改为从0~x 以用于快速hash
			//EC_Node *pNode = new EC_Node(p->getId(),p,ec_node_x,ec_node_y,p->get_users(),p->get_pois());
			EC_Node *pNode = new EC_Node(node_id++, p, ec_node_x, ec_node_y, p->get_users(), p->get_pois());
            nodes.push_back(pNode);
            nodes_map[p->getId()] = pNode;
        }
        cout << "edge graph nodes initialed !!" << endl;
        //遍历每个顶点，计算边簇图中的联系
        EC_Node *pNode1,*pNode2;
        EC_Edge *pEdge=nullptr;
        vector<Node*> src_nodes = p_graph->getNodes();
		size_t src_node_size = src_nodes.size();
        cout << src_nodes.size() << " src_graph nodes " << endl;
        for(int i=0;i<src_node_size;i++){
            int num_of_adj_edges = src_nodes[i]->getAdjEdges().size();
            if(num_of_adj_edges<2) continue; // 叶子顶点
            // 从n条临近边中选择两条，增加为两天边对应的顶点添加联系
            for(int t=0;t<num_of_adj_edges-1;t++){
                for(int k=t+1;k<num_of_adj_edges;k++){
                    long long ec_node1_id = src_nodes[i]->getAdjEdges()[t]->getId();
                    long long ec_node2_id = src_nodes[i]->getAdjEdges()[k]->getId();
                    pNode1 = getECNode(ec_node1_id);
                    pNode2 = getECNode(ec_node2_id);
                    cnt_of_ec_edges++;
                    // 创建边图中的边，用于以后可能的统计分析

					pEdge = new EC_Edge(cnt_of_ec_edges, src_nodes[i], pNode1, pNode2);
                    edges.push_back(pEdge);
					
                    pNode1->add_adj_node(pNode2,pEdge);
                    pNode2->add_adj_node(pNode1,pEdge);

                }
            }
        }
        cout << "edge graph init done" << endl;
    }
    ~EC_Graph()
    {
        delete(p_graph);
        for(int i=0;i<nodes.size();i++){
            delete(nodes[i]);
        }
        for(int i=0;i<edges.size();i++){
            delete(edges[i]);
        }
    }
    EC_Node* getECNode(long long node_id)
    {
        if(nodes_map.find(node_id)!=nodes_map.end()){
            return nodes_map[node_id];
        }
    }
    const vector<EC_Node*>& get_nodes()
    {
        return nodes;
    }
    const vector<EC_Edge*>& get_edges()
    {
        return edges;
    }
private:
    vector<EC_Edge*> edges;
    vector<EC_Node*> nodes;
    map<long long,EC_Node*> nodes_map;
    long cnt_of_ec_edges;
public:
    Graph *p_graph;
};
#endif //C_PLUS_PLUS_PRACTICE_EDGE_CLUSTER_GRAPH_H
