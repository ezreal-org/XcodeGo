#ifndef LPPA_SA_H
#define LPPA_SA_H

/*
SA算法隐私模型为 k,θ安全, 采用泰森多边形进行兴趣点划分
整个流程暴露的是Asuit,匿名算法在客户端执行
当用户所在边为敏感区域时执行算法1
当用户所在边为非敏感区域时执行算法2

算法1：加入所有邻接的非敏感语义或交叉点，但不存在非敏感语义时，加入敏感度最小的敏感语义并执行算法2
ps: 当用户处于非敏感区域时，加入非敏感区域或交叉点都能保证匿名区域非敏感
算法2：加入所有邻接的非敏感语义，当不存在时随机选择一个交叉点中，当交叉点不存在时，选择敏感度最小的敏感语义
ps: 执行算法2意味着，匿名区域包含敏感兴趣点，匿名集扩展时应考虑候选集语义，`不能把非敏感语义和交叉点看的同样重要`，而加入所有交叉点

不能保证l多样性
为了适应新的隐私目标、方便比较查询效率：
采用第三方进行匿名，不影响算法本质
每次匿名扩展时，加入一条临近边而不是一个位置
s_type_poi_pop/poi_pop_all >s_require 的定义为敏感语义
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
		srand(time(NULL)); //随机选择一个空白语义时，会引入一定的随机性
		//打印当前地图状态
	}
	~Lppa_sa()
	{
		delete[]is_edge_candidate;
		delete[]is_inner_node;
	}
	//保护算法
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
		//统计
		//这一块将不计入匿名时间，只是用于分析算法性能
		//开放节点处于运行时未决状态,只作为评价指标
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
				for (int j = 0; j < adj_edges.size(); j++) { //判断所有临近边是否都在匿名集
					if (!vector_find(cloak_set, adj_edges[j])) { //没找到,还计算有多少在？
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
	//为每个用户
	vector<Edge*> sa(LBS_User *&pu, Edge *&pe)
	{
		is_success = false;
		k = 0, l = 0;
		accumulate_svalue = 0, accumulate_pop = 0;
		memset(is_edge_candidate, false, p_graph->getEdges().size());
		memset(is_inner_node, false, p_graph->getNodes().size());
		vector<Edge*> cloak_set;
		map<Edge*, pair<double, double>> candidate_map;
		is_edge_candidate[pe->getId()] = true; // 加这个是为了第一次运行算法时，用户所在边不会重新被选中
		is_success = add_edge_to_cloakset(pu, pe, cloak_set, candidate_map); //初始匿名集为用户所在边
		is_edge_candidate[pe->getId()] = false;
		if (accumulate_pop<0.0001 || accumulate_svalue / accumulate_pop <= pu->get_s()) { //非敏感边
			algorithm1(pu, cloak_set, candidate_map);
		}
		else { //敏感边
			algorithm2(pu, cloak_set, candidate_map);
		}
		return cloak_set;
	}

	//算法1 为非敏感边匿名
	//加入非敏感语义或空白，不存在时加入敏感值最小的敏感语义，执行算法2
	void algorithm1(LBS_User *&pu, vector<Edge*> &cloak_set, map<Edge*, pair<double, double>> &candidate_map)
	{
		for (int i = cloak_set.size(); i < l_max && !is_success; i++) {
			map<Edge*, pair<double, double>>::iterator it_candidate, it_minimal_svalue;
			double minimal_svalue = 0x7fffffff;
			if (candidate_map.size() < 1) break;
			for (it_candidate = candidate_map.begin(); it_candidate != candidate_map.end(); it_candidate++) { //已经计算了的就不用计算
				double candidate_svalue = it_candidate->second.first;
				double candidate_pop = it_candidate->second.second;
				if (candidate_pop < 0.0001) { //空语义路段,算法1中直接加入
					break;
				}
				if (candidate_svalue / candidate_pop <= pu->get_s()) { //选择非敏感poi
					break;
				}
				if (candidate_svalue < minimal_svalue) {
					minimal_svalue = candidate_svalue;
					it_minimal_svalue = it_candidate;
				}
			}
			if (it_candidate != candidate_map.end()) { //找到非敏感poi或空语义路段
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

	//算法2 为敏感边匿名
	//加入非敏感语义，不存在时随机选择空白语义，仍然不存在时，选择敏感值最小的
	void algorithm2(LBS_User *&pu, vector<Edge*> &cloak_set, map<Edge*, pair<double, double>> &candidate_map)
	{
		vector<Edge*> none_semantic_set;
		for (int i = cloak_set.size(); i < l_max && !is_success; i++) {
			map<Edge*, pair<double, double>>::iterator it_candidate, it_minimal_svalue;
			double minimal_svalue = 0x7fffffff;
			if (candidate_map.size() < 1 && none_semantic_set.size()<1) break;
			for (it_candidate = candidate_map.begin(); it_candidate != candidate_map.end();) { //已经计算了的就不用计算
				double candidate_svalue = it_candidate->second.first;
				double candidate_pop = it_candidate->second.second;
				if (candidate_pop < 0.0001) { //空语义边
					none_semantic_set.push_back(it_candidate->first);
					it_candidate = candidate_map.erase(it_candidate); //空路段放vector里，用于随机选择;但标识数组is_edge_candidate标识的该边仍然为true
					continue; // erase函数返回下一个可用迭代器
				}
				if (candidate_svalue / candidate_pop <= pu->get_s()) { //选择非敏感poi
					break;
				}
				if (candidate_svalue < minimal_svalue) {
					minimal_svalue = candidate_svalue;
					it_minimal_svalue = it_candidate;
				}
				it_candidate++; //迭代器增长
			}
			if (it_candidate != candidate_map.end()) { //找到非敏感poi
				is_success = add_edge_to_cloakset(pu, it_candidate->first, cloak_set, candidate_map);
				is_edge_candidate[it_candidate->first->getId()] = false; // erase后迭代器将失效
				candidate_map.erase(it_candidate);
			}
			else {
				int ns_size = none_semantic_set.size();
				if (ns_size > 0) { //存在空白语义位置
					int selected_index = rand() % ns_size;
					Edge *pnone = none_semantic_set[selected_index];
					is_success = add_edge_to_cloakset(pu, pnone, cloak_set, candidate_map);
					is_edge_candidate[pnone->getId()] = false;
					none_semantic_set.erase(none_semantic_set.begin() + selected_index);
				}
				else { //选择敏感值最小的候选边
					is_success = add_edge_to_cloakset(pu, it_minimal_svalue->first, cloak_set, candidate_map);
					is_edge_candidate[it_minimal_svalue->first->getId()] = false;
					candidate_map.erase(it_minimal_svalue);
				}
			}
		}//end for
	}
	bool add_edge_to_cloakset(LBS_User *&pu, Edge *new_edge, vector<Edge*> &cloak_set, map<Edge*, pair<double, double>> &candidate_map)
	{
		//更新匿名集和候选集
		const vector<double> &sensitive_vals = pu->get_sensitive_vals();
		cloak_set.push_back(new_edge);
		Node *pn1, *pn2;
		pn1 = new_edge->getNode1(); //加入一条边,增加一个新顶点或者不变
		pn2 = new_edge->getNode2(); //只影响刚加入的部分
		if (!is_inner_node[pn1->getIndex()]) {
			is_inner_node[pn1->getIndex()] = true;
			vector<Edge*> &adj_edges1 = pn1->getAdjEdges();
			for (int i = 0; i < adj_edges1.size(); i++) { //计算邻居边对该用户的敏感情况
				Edge *candidate_edge = adj_edges1[i];
				if (!is_edge_candidate[candidate_edge->getId()]) { //匿名集扩展加入一个新顶点时，该顶点不可能邻接已经选的边，因为已选边的两个顶点都必定加入了inner_node
					double candidate_edge_svalue = 0.0, candidate_edge_pop = 0.0;
					const vector<Poi*> &e_pois = candidate_edge->get_pois();
					for (int j = 0; j < e_pois.size(); j++) { //多个兴趣点
						int poi_type = (int)e_pois[j]->get_type();
						candidate_edge_svalue += (sensitive_vals[poi_type] * e_pois[j]->get_pop());
						candidate_edge_pop += e_pois[j]->get_pop();
					}
					candidate_map[candidate_edge] = make_pair(candidate_edge_svalue, candidate_edge_pop); // 用于判断敏感率 < θ
					is_edge_candidate[candidate_edge->getId()] = true;
				}
			}
		}
		if (!is_inner_node[pn2->getIndex()]) {
			is_inner_node[pn2->getIndex()] = true;
			vector<Edge*> &adj_edges2 = pn2->getAdjEdges();
			for (int i = 0; i < adj_edges2.size(); i++) { //计算邻居边对该用户的敏感情况
				Edge *candidate_edge = adj_edges2[i];
				if (!is_edge_candidate[candidate_edge->getId()]) {
					double candidate_edge_svalue = 0.0, candidate_edge_pop = 0.0;
					const vector<Poi*> &e_pois = candidate_edge->get_pois();
					for (int j = 0; j < e_pois.size(); j++) { //多个兴趣点
						int poi_type = (int)e_pois[j]->get_type();
						candidate_edge_svalue += (sensitive_vals[poi_type] * e_pois[j]->get_pop());
						candidate_edge_pop += e_pois[j]->get_pop();
					}
					candidate_map[candidate_edge] = make_pair(candidate_edge_svalue, candidate_edge_pop); // 用于判断敏感率 < θ
					is_edge_candidate[candidate_edge->getId()] = true;
				}
			}
		}
		//计算当前状态
		k += new_edge->get_users().size();
		l++;
		double candidate_svalue = 0.0, candidate_pop = 0.0;
		if (candidate_map.find(new_edge) == candidate_map.end()) { //第一次执行或空语义边
			const vector<Poi*> &e_pois = new_edge->get_pois();
			for (int j = 0; j < e_pois.size(); j++) { //多个兴趣点
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
	bool is_success; //记录当前匿名的用户，是否匿名成功
	bool *is_edge_candidate;
	bool *is_inner_node;

	vector<bool> v_success;
	vector<LBS_User*> users;
	vector<vector<Edge*>> vv_cloak_sets;
	//统计信息
	double anonymization_time_total;
	int cnt_of_success;
	int cnt_of_failure;
	vector<int> open_vetex_cnt; //将用于以权值减去环数
	int l_max; //匿名集最多选择l_max条边,否则失败
	Graph *p_graph;
};

#endif