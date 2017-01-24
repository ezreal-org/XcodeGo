//
// Created by jayce on 09/11/2016.
//

#ifndef C_PLUS_PLUS_PRACTICE_GRAPH_NODE_H
#define C_PLUS_PLUS_PRACTICE_GRAPH_NODE_H
#include <vector>
using namespace std;
class Edge;

class Node{
public:
    Node(int index,long long id,double x,double y)
    {
        this->id = id;
        this->x = x;
        this->y = y;
		this->index = index;
        numOfEdges = 0;
    }
    void addEdge(Edge *ep)
    {
        edges.push_back(ep);
        numOfEdges++;
    }
	int getIndex()
	{
		return index;
	}
    long long getId()
    {
        return id;
    }
    double getX()
    {
        return x;
    }
    double getY()
    {
        return y;
    }
    vector<Edge*>& getAdjEdges()
    {
        return edges;
    }
private:
	int index; //用于方便hash
    long long id;
    double x,y;
    int numOfEdges;
    vector<Edge*> edges;
};

#endif //C_PLUS_PLUS_PRACTICE_GRAPH_NODE_H
