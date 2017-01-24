#ifndef LPPA_PLPCA_H
#define LPPA_PLPCA_H
#include "../edge_cluster_graph.h"
#include "../lbs_user.h"
#include "../poi.h"
#include "util.h"
#include <vector>
#include <set>
#include <map>
using namespace std;

/*
	�ڱ�ͼ��ִ���㷨��������ȵķ�����һ����
	�һ�����Ѧ������,���νṹ
	���繥���߲�֪����ͼ��ת���������㷨�����ǹ�����
	�е�ͼ����Ϣ��ÿ��·�ε�������Ϣ�������߻��������
	������Ч��ִ�й���������ʹ�ñ�ͼ�������ҵ�һ������Χ��
	�ھӱߺ���������������Ϣ��

	Ϊ����Ƚ������µ��������蹥����֪�������㷨�ͱ�ͼ��ת����
	������Ƚ��б���,���� l_require < L < l_max �֣�ȡ������˽Ҫ��
	����С��
*/
class Lppa_plpca
{
public:
	Lppa_plpca(EC_Graph *p_graph)
	{
		this->p_graph = p_graph;
		anonymization_time_total = 0;
		cnt_of_failure = 0;
		cnt_of_success = 0;
		l_max = 7;
	}
	void lpp()
	{
		vector<EC_Node*> nodes = p_graph->get_nodes();
		LBS_User *pu = nullptr;
		clock_t start, finish;
		start = clock();
		for (int i = 0; i < nodes.size(); i++) {
			for (int j = 0; j < nodes[i]->get_users().size(); j++) {
				pu = nodes[i]->get_users()[j];
				users.push_back(nodes[i]->get_users()[j]);
				vv_cloaks.push_back(plpca(pu, nodes[i]));
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
		vector<Edge*>::iterator it_cloak_set;
		Node *pn1, *pn2;
		int all_openvetex_cnt = 0, all_edge_size = 0;

		int user_cnt = vv_cloaks.size();
		for (int i = 0; i < user_cnt; i++) {
			cloak_set.clear();
			for (int j = 0; j < vv_cloaks[i].size(); j++)
				cloak_set.push_back(vv_cloaks[i][j]->get_src_edge());
			if (cloak_set.size() < 1) { //����ʧ��,û���ҵ�������ͳ������l_max
				all_edge_size += l_max;
				all_openvetex_cnt += l_max;
				continue;
			}
			node_set.clear();
			//��ʼ����ض��㼯
			for (int kk = 0; kk < cloak_set.size(); kk++) {
				Node *pn1, *pn2;
				pn1 = cloak_set[kk]->getNode1();
				pn2 = cloak_set[kk]->getNode2();
				if (node_set.find(pn1) == node_set.end()) {
					node_set.insert(pn1);
				}
				if (node_set.find(pn2) == node_set.end()) {
					node_set.insert(pn2);
				}
			}
			all_edge_size += cloak_set.size();
			int cnt_of_openvetex = 0;
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

	vector<EC_Node*> plpca(LBS_User *pu,EC_Node * const pn) //Ϊ���û�ִ���㷨
	{
		map<pair<int,EC_Node*>, pair<int, EC_Node*>> pre_map; //Ϊÿ���û�ִ�������㷨ʱ����¼ǰ��,int��ʾλ�ã�EC_Node*��ʾĳ���ڵ�
		vector<vector<EC_Node*>> vv_circles; //�õ������
		vector<EC_Node*> expand_queue;
		vector<EC_Node*> v_cloak; //������
		int lay_now,size_of_this_lay; // accumulate_expand_index �����ƹ�ϣ������,ͬһ��EC_Node����ǰ���ڲ�ͬ�㲻ͬλ�õ�ǰ����һ��;������չʱ�ڼ��������ö���
		int accumulate_queue_index = 0, accumulate_expand_index = 0;
		expand_queue.push_back(pn);
		pre_map.clear();
		pre_map[make_pair(0,pn)] = make_pair(-1,pn); //���ǰ��Ϊ��
		size_of_this_lay = 1;
		//������л�
		for (lay_now=0; lay_now < l_max; lay_now++) {
			int size_of_next_lay = 0;
			for (int i = 0; i < size_of_this_lay; i++) {
				EC_Node *node_now = expand_queue[accumulate_queue_index];
				vector<EC_Node*> &adj_nodes = node_now->get_adj_nodes();
				for (int j = 0; j < adj_nodes.size(); j++) {
					if (adj_nodes[j] == pn && lay_now>1) { //�ҵ�һ����,��������������
						EC_Node* p_node = node_now;
						vector<EC_Node*> simple_circle;
						simple_circle.push_back(pn);
						simple_circle.push_back(node_now);
						pair<int, EC_Node*> p_node_info = pre_map[make_pair(accumulate_queue_index, node_now)];
						while (p_node_info.second != pn) {
							simple_circle.push_back(p_node_info.second);
							p_node_info = pre_map[p_node_info];
						}
						vv_circles.push_back(simple_circle);
					}
					else{
						//�����Ķ��㣬Ϊ�ﵽ���Ľṹ��Ӧ���ڹ�����ȵ�ĳ����չ·���ϲ��ظ�
						if (adj_nodes[j] == pn) continue; //����Ϊ��ֻ�������ߵ����Σ���ͨ����һ�㶥����ҵ�������
						pair<int, EC_Node*> p_node_info = pre_map[make_pair(accumulate_queue_index, node_now)];
						while (p_node_info.second != pn) { 
							if (p_node_info.second == adj_nodes[j])
								break;
							p_node_info = pre_map[p_node_info];
						}
						if (p_node_info.second == pn) {  //·���ϲ��ظ�
							expand_queue.push_back(adj_nodes[j]);
							pre_map[make_pair(++accumulate_expand_index, adj_nodes[j])] = make_pair(accumulate_queue_index, node_now);
							size_of_next_lay++;
						}
					}
				}
				accumulate_queue_index++;
			}
			size_of_this_lay = size_of_next_lay;
		}// end for
		//ȷ������Ҫ�����С��
		int size_of_circle = vv_circles.size();
		//����������
		for (int i = 0; i < size_of_circle-1; i++) {
			int minimal_index = i;
			vector<EC_Node*> temp;
			for (int j = i+1; j < size_of_circle; j++) {
				if (vv_circles[j].size() < vv_circles[minimal_index].size()) 
					minimal_index = j;
			}
			if (minimal_index != i) {
				temp = vv_circles[i];
				vv_circles[i] = vv_circles[minimal_index];
				vv_circles[minimal_index] = temp;
			}
		}
		//����С����Ҫ��Ļ�
		vector<double> sensitive_vals = pu->get_sensitive_vals();
		is_success = false;
		for (int i = 0; i < size_of_circle; i++) {
			int edge_size = vv_circles[i].size();
			if (edge_size < pu->get_l()) continue; //���������û�����l
			double accumulate_svalue = 0.0, accumulate_pop = 0.0;
			int k_value = 0, l_value = edge_size;
			for (int j = 0; j < edge_size; j++) {
				Edge *p_edge = vv_circles[i][j]->get_src_edge();
				k_value += p_edge->get_users().size();
				vector<Poi*> e_pois = p_edge->get_pois();
				for (int m = 0; m < e_pois.size(); m++) { //�����Ȥ��
					int poi_type = (int)e_pois[m]->get_type();
					accumulate_svalue += (sensitive_vals[poi_type] * e_pois[m]->get_pop());
					accumulate_pop += e_pois[m]->get_pop();
				}
			}
			if (k_value >= pu->get_k() && l_value >= pu->get_l() && (accumulate_pop < 0.001 || (accumulate_pop >= 0.001 && accumulate_svalue / accumulate_pop <= pu->get_s()))) {
				for (int j = 0; j < edge_size; j++) {
					v_cloak.push_back(vv_circles[i][j]);
				}
				is_success = true;
				break;
			}
		}
		return v_cloak;
	}
	vector<LBS_User*> get_users()
	{
		return users;
	}
	vector<vector<EC_Node*>> get_vv_cloak()
	{
		return vv_cloaks;
	}
	vector<bool> get_v_success()
	{
		return v_success;
	}
private:
	vector<vector<EC_Node*>> vv_cloaks;
	bool is_success; //��¼���û������Ƿ�ɹ�
	vector<bool> v_success;
	vector<LBS_User*> users;
	//ͳ����Ϣ
	double anonymization_time_total;
	int cnt_of_success;
	int cnt_of_failure;
	vector<int> open_vetex_cnt; //��������Ȩֵ��ȥ����
	int l_max; //���������ѡ��l_max����,����ʧ��
	EC_Graph *p_graph;
};
#endif
