#ifndef LPPA_SA_H
#define LPPA_SA_H

/*
SA�㷨��˽ģ��Ϊ k,�Ȱ�ȫ, ����̩ɭ����ν�����Ȥ�㻮��
�������̱�¶����Asuit,�����㷨�ڿͻ���ִ��
���û����ڱ�Ϊ��������ʱִ���㷨1
���û����ڱ�Ϊ����������ʱִ���㷨2

�㷨1�����������ڽӵķ���������򽻲�㣬�������ڷ���������ʱ���������ж���С���������岢ִ���㷨2
ps: ���û����ڷ���������ʱ���������������򽻲�㶼�ܱ�֤�������������
�㷨2�����������ڽӵķ��������壬��������ʱ���ѡ��һ��������У�������㲻����ʱ��ѡ�����ж���С����������
ps: ִ���㷨2��ζ�ţ������������������Ȥ�㣬��������չʱӦ���Ǻ�ѡ�����壬`���ܰѷ���������ͽ���㿴��ͬ����Ҫ`�����������н����

���ܱ�֤l������
Ϊ����Ӧ�µ���˽Ŀ�ꡢ����Ƚϲ�ѯЧ�ʣ�
���õ�����������������Ӱ���㷨����
ÿ��������չʱ������һ���ٽ��߶�����һ��λ��
s_type_poi_pop/poi_pop_all >s_require �Ķ���Ϊ��������
*/

#include "../graph.h"
#include "util.h"
#include <ctime>
#include <cstdlib>
#include <vector>
#include <set>
#include <map>
using namespace std;

class Lppa_sa
{
public:
	Lppa_sa(Graph *&p_graph)
	{
		this->p_graph = p_graph;
		anonymization_time_total = 0;
		cnt_of_failure = 0;
		cnt_of_success = 0;
		l_max = 30;
		srand(time(NULL)); //���ѡ��һ���հ�����ʱ��������һ���������
		//��ӡ��ǰ��ͼ״̬
	}
	~Lppa_sa()
	{
		delete[]is_edge_candidate;
		delete[]is_inner_node;
	}
	//�����㷨
	void lpp()
	{
		vector<Edge*> edges = p_graph->getEdges();
		LBS_User *pu = nullptr;
		clock_t start, finish;
		is_edge_candidate = new bool[edges.size()];
		is_inner_node = new bool[p_graph->getNodes().size()];
		start = clock();
		for (int i = 0; i < edges.size(); i++) {
			for (int j = 0; j < edges[i]->get_users().size(); j++) {
				pu = edges[i]->get_users()[j];
				users.push_back(edges[i]->get_users()[j]);
				vv_cloak_sets.push_back(sa(pu, edges[i]));
				is_success ? cnt_of_success++ : cnt_of_failure++;
				v_success.push_back(is_success);
			}
		}
		finish = clock();
		double time_cost = (double)(finish - start) / CLOCKS_PER_SEC;
		cout << "total time consume:" << time_cost << endl;
		cout << "success:" << cnt_of_success << endl;
		cout << "failure:" << cnt_of_failure << endl;
		//ͳ��
		//��һ�齫����������ʱ�䣬ֻ�����ڷ����㷨����
		//���Žڵ㴦������ʱδ��״̬,ֻ��Ϊ����ָ��
		cout << "calculate open vetex begin:" << endl;
		set<Node*> node_set;
		set<Node*>::iterator it_node_set;
		vector<Edge*> adj_edges, cloak_set;
		int all_openvetex_cnt = 0, all_edge_size = 0;

		int user_cnt = vv_cloak_sets.size();
		for (int i = 0; i < vv_cloak_sets.size(); i++) {
			cloak_set.clear();
			for (int j = 0; j < vv_cloak_sets[i].size(); j++) {
				cloak_set.push_back(vv_cloak_sets[i][j]);
			}
			node_set.clear();
			//init inner_node set
			for (int j = 0; j < cloak_set.size(); j++) {
				Node *pn1, *pn2;
				pn1 = cloak_set[j]->getNode1();
				pn2 = cloak_set[j]->getNode2();
				if (node_set.find(pn1) == node_set.end()) {
					node_set.insert(pn1);
				}
				if (node_set.find(pn2) == node_set.end()) {
					node_set.insert(pn2);
				}
			}
			all_edge_size += cloak_set.size();
			int cnt_of_circle = 0, cnt_of_openvetex = 0;

			for (it_node_set = node_set.begin(); it_node_set != node_set.end(); it_node_set++) {
				adj_edges = (*it_node_set)->getAdjEdges();
				for (int j = 0; j < adj_edges.size(); j++) { //�ж������ٽ����Ƿ���������
					if (!vector_find(cloak_set, adj_edges[j])) { //û�ҵ�,�������ж����ڣ�
						cnt_of_openvetex++;
						break;
					}
				}
			}

			open_vetex_cnt.push_back(cnt_of_openvetex);
			all_openvetex_cnt += cnt_of_openvetex;
			//cout << "open vetex: "<< cnt_of_openvetex << endl;
			if (cnt_of_openvetex > 60) cout << "###" << endl;
		}
		cout << "average openvetex: " << (double)all_openvetex_cnt / user_cnt << endl;
		cout << "average cloak size: " << (double)all_edge_size / user_cnt << endl;

	}
	//Ϊÿ���û�
	vector<Edge*> sa(LBS_User *&pu, Edge *&pe)
	{
		is_success = false;
		k = 0, l = 0;
		accumulate_svalue = 0, accumulate_pop = 0;
		memset(is_edge_candidate, false, p_graph->getEdges().size());
		memset(is_inner_node, false, p_graph->getNodes().size());
		vector<Edge*> cloak_set;
		map<Edge*, pair<double, double>> candidate_map;
		is_edge_candidate[pe->getId()] = true; // �������Ϊ�˵�һ�������㷨ʱ���û����ڱ߲������±�ѡ��
		is_success = add_edge_to_cloakset(pu, pe, cloak_set, candidate_map); //��ʼ������Ϊ�û����ڱ�
		is_edge_candidate[pe->getId()] = false;
		if (accumulate_pop<0.0001 || accumulate_svalue / accumulate_pop <= pu->get_s()) { //�����б�
			algorithm1(pu, cloak_set, candidate_map);
		}
		else { //���б�
			algorithm2(pu, cloak_set, candidate_map);
		}
		return cloak_set;
	}

	//�㷨1 Ϊ�����б�����
	//��������������հף�������ʱ��������ֵ��С���������壬ִ���㷨2
	void algorithm1(LBS_User *&pu, vector<Edge*> &cloak_set, map<Edge*, pair<double, double>> &candidate_map)
	{
		for (int i = cloak_set.size(); i < l_max && !is_success; i++) {
			map<Edge*, pair<double, double>>::iterator it_candidate, it_minimal_svalue;
			double minimal_svalue = 0x7fffffff;
			if (candidate_map.size() < 1) break;
			for (it_candidate = candidate_map.begin(); it_candidate != candidate_map.end(); it_candidate++) { //�Ѿ������˵ľͲ��ü���
				double candidate_svalue = it_candidate->second.first;
				double candidate_pop = it_candidate->second.second;
				if (candidate_pop < 0.0001) { //������·��,�㷨1��ֱ�Ӽ���
					break;
				}
				if (candidate_svalue / candidate_pop <= pu->get_s()) { //ѡ�������poi
					break;
				}
				if (candidate_svalue < minimal_svalue) {
					minimal_svalue = candidate_svalue;
					it_minimal_svalue = it_candidate;
				}
			}
			if (it_candidate != candidate_map.end()) { //�ҵ�������poi�������·��
				is_success = add_edge_to_cloakset(pu, it_candidate->first, cloak_set, candidate_map);
				is_edge_candidate[it_candidate->first->getId()] = false;
				candidate_map.erase(it_candidate);
			}
			else {
				is_success = add_edge_to_cloakset(pu, it_minimal_svalue->first, cloak_set, candidate_map);
				is_edge_candidate[it_minimal_svalue->first->getId()] = false;
				candidate_map.erase(it_minimal_svalue);
				return algorithm2(pu, cloak_set, candidate_map);
			}
		}//end for
	}

	//�㷨2 Ϊ���б�����
	//������������壬������ʱ���ѡ��հ����壬��Ȼ������ʱ��ѡ������ֵ��С��
	void algorithm2(LBS_User *&pu, vector<Edge*> &cloak_set, map<Edge*, pair<double, double>> &candidate_map)
	{
		vector<Edge*> none_semantic_set;
		for (int i = cloak_set.size(); i < l_max && !is_success; i++) {
			map<Edge*, pair<double, double>>::iterator it_candidate, it_minimal_svalue;
			double minimal_svalue = 0x7fffffff;
			if (candidate_map.size() < 1 && none_semantic_set.size()<1) break;
			for (it_candidate = candidate_map.begin(); it_candidate != candidate_map.end();) { //�Ѿ������˵ľͲ��ü���
				double candidate_svalue = it_candidate->second.first;
				double candidate_pop = it_candidate->second.second;
				if (candidate_pop < 0.0001) { //�������
					none_semantic_set.push_back(it_candidate->first);
					it_candidate = candidate_map.erase(it_candidate); //��·�η�vector��������ѡ��;����ʶ����is_edge_candidate��ʶ�ĸñ���ȻΪtrue
					continue; // erase����������һ�����õ�����
				}
				if (candidate_svalue / candidate_pop <= pu->get_s()) { //ѡ�������poi
					break;
				}
				if (candidate_svalue < minimal_svalue) {
					minimal_svalue = candidate_svalue;
					it_minimal_svalue = it_candidate;
				}
				it_candidate++; //����������
			}
			if (it_candidate != candidate_map.end()) { //�ҵ�������poi
				is_success = add_edge_to_cloakset(pu, it_candidate->first, cloak_set, candidate_map);
				is_edge_candidate[it_candidate->first->getId()] = false; // erase���������ʧЧ
				candidate_map.erase(it_candidate);
			}
			else {
				int ns_size = none_semantic_set.size();
				if (ns_size > 0) { //���ڿհ�����λ��
					int selected_index = rand() % ns_size;
					Edge *pnone = none_semantic_set[selected_index];
					is_success = add_edge_to_cloakset(pu, pnone, cloak_set, candidate_map);
					is_edge_candidate[pnone->getId()] = false;
					none_semantic_set.erase(none_semantic_set.begin() + selected_index);
				}
				else { //ѡ������ֵ��С�ĺ�ѡ��
					is_success = add_edge_to_cloakset(pu, it_minimal_svalue->first, cloak_set, candidate_map);
					is_edge_candidate[it_minimal_svalue->first->getId()] = false;
					candidate_map.erase(it_minimal_svalue);
				}
			}
		}//end for
	}
	bool add_edge_to_cloakset(LBS_User *&pu, Edge *new_edge, vector<Edge*> &cloak_set, map<Edge*, pair<double, double>> &candidate_map)
	{
		//�����������ͺ�ѡ��
		const vector<double> &sensitive_vals = pu->get_sensitive_vals();
		cloak_set.push_back(new_edge);
		Node *pn1, *pn2;
		pn1 = new_edge->getNode1(); //����һ����,����һ���¶�����߲���
		pn2 = new_edge->getNode2(); //ֻӰ��ռ���Ĳ���
		if (!is_inner_node[pn1->getIndex()]) {
			is_inner_node[pn1->getIndex()] = true;
			vector<Edge*> &adj_edges1 = pn1->getAdjEdges();
			for (int i = 0; i < adj_edges1.size(); i++) { //�����ھӱ߶Ը��û����������
				Edge *candidate_edge = adj_edges1[i];
				if (!is_edge_candidate[candidate_edge->getId()]) { //��������չ����һ���¶���ʱ���ö��㲻�����ڽ��Ѿ�ѡ�ıߣ���Ϊ��ѡ�ߵ��������㶼�ض�������inner_node
					double candidate_edge_svalue = 0.0, candidate_edge_pop = 0.0;
					const vector<Poi*> &e_pois = candidate_edge->get_pois();
					for (int j = 0; j < e_pois.size(); j++) { //�����Ȥ��
						int poi_type = (int)e_pois[j]->get_type();
						candidate_edge_svalue += (sensitive_vals[poi_type] * e_pois[j]->get_pop());
						candidate_edge_pop += e_pois[j]->get_pop();
					}
					candidate_map[candidate_edge] = make_pair(candidate_edge_svalue, candidate_edge_pop); // �����ж������� < ��
					is_edge_candidate[candidate_edge->getId()] = true;
				}
			}
		}
		if (!is_inner_node[pn2->getIndex()]) {
			is_inner_node[pn2->getIndex()] = true;
			vector<Edge*> &adj_edges2 = pn2->getAdjEdges();
			for (int i = 0; i < adj_edges2.size(); i++) { //�����ھӱ߶Ը��û����������
				Edge *candidate_edge = adj_edges2[i];
				if (!is_edge_candidate[candidate_edge->getId()]) {
					double candidate_edge_svalue = 0.0, candidate_edge_pop = 0.0;
					const vector<Poi*> &e_pois = candidate_edge->get_pois();
					for (int j = 0; j < e_pois.size(); j++) { //�����Ȥ��
						int poi_type = (int)e_pois[j]->get_type();
						candidate_edge_svalue += (sensitive_vals[poi_type] * e_pois[j]->get_pop());
						candidate_edge_pop += e_pois[j]->get_pop();
					}
					candidate_map[candidate_edge] = make_pair(candidate_edge_svalue, candidate_edge_pop); // �����ж������� < ��
					is_edge_candidate[candidate_edge->getId()] = true;
				}
			}
		}
		//���㵱ǰ״̬
		k += new_edge->get_users().size();
		l++;
		double candidate_svalue = 0.0, candidate_pop = 0.0;
		if (candidate_map.find(new_edge) == candidate_map.end()) { //��һ��ִ�л�������
			const vector<Poi*> &e_pois = new_edge->get_pois();
			for (int j = 0; j < e_pois.size(); j++) { //�����Ȥ��
				int poi_type = (int)e_pois[j]->get_type();
				candidate_svalue += (sensitive_vals[poi_type] * e_pois[j]->get_pop());
				candidate_pop += e_pois[j]->get_pop();
			}
		}
		else {
			candidate_svalue = candidate_map[new_edge].first;
			candidate_pop = candidate_map[new_edge].second;
		}
		accumulate_svalue += candidate_svalue;
		accumulate_pop += candidate_pop;
		if (k < pu->get_k() || l < pu->get_l() || (accumulate_pop>0.001 &&accumulate_svalue / accumulate_pop > pu->get_s())) return false;
		//cout << "find satisfied" << endl;
		return true;
	}
	vector<LBS_User*> get_users()
	{
		return users;
	}
	vector<vector<Edge*>> get_vv_cloaks()
	{
		return vv_cloak_sets;
	}
	vector<bool> get_v_success()
	{
		return v_success;
	}
private:
	int k, l;
	double accumulate_svalue = 0, accumulate_pop = 0;
	bool is_success; //��¼��ǰ�������û����Ƿ������ɹ�
	bool *is_edge_candidate;
	bool *is_inner_node;

	vector<bool> v_success;
	vector<LBS_User*> users;
	vector<vector<Edge*>> vv_cloak_sets;
	//ͳ����Ϣ
	double anonymization_time_total;
	int cnt_of_success;
	int cnt_of_failure;
	vector<int> open_vetex_cnt; //��������Ȩֵ��ȥ����
	int l_max; //���������ѡ��l_max����,����ʧ��
	Graph *p_graph;
};

#endif