//
// Created by jayce on 09/11/2016.
//

#ifndef C_PLUS_PLUS_PRACTICE_GRAPH_EDGE_H
#define C_PLUS_PLUS_PRACTICE_GRAPH_EDGE_H
#include "poi.h"
#include "lbs_user.h"

class Node;

class Edge{
public:
    Edge(long long eid,Node* p1,Node *p2, int edgeClass, double len)
    {
        this->eid = eid;
        this->node1 = p1;
        this->node2 = p2;
        this->edgeClass = edgeClass;
        this->len = len;
    }
    long long getId()
    {
        return eid;
    }
    Node* getNode1()
    {
        return node1;
    }
    Node* getNode2()
    {
        return node2;
    }
    int getEdgeClass()
    {
        return edgeClass;
    }
    double getEdgeLen()
    {
        return len;
    }
	void add_user(LBS_User *user)
	{
		users.push_back(user);
	}
	void add_poi(Poi *poi)
	{
		pois.push_back(poi);
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
    long long eid;
    Node *node1;
    Node *node2;
    int edgeClass;
    double len;
	vector<LBS_User*> users;
	vector<Poi*> pois;
};

#endif //C_PLUS_PLUS_PRACTICE_GRAPH_EDGE_H
