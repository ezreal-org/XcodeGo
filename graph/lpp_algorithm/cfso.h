#ifndef LPPA_CFSO_H
#define LPPA_CFSO_H

/*
SA算法隐私模型为 k,θ安全, 采用泰森多边形进行兴趣点划分
整个流程暴露的是Asuit,匿名算法在客户端执行
在用户扩大匿名区域的过程中,首先随机选择非敏感位置-语意缺失值
再选择道路交叉点，最后选择流行度最低的敏感语义

算法本质为，每次加入一个临近兴趣点，直到满足隐私模型
不能保证l多样性
为了适应新的隐私目标、方便比较查询效率：
采用第三方进行匿名，不影响算法本质
每次匿名扩展时，加入一条临近边而不是一个位置
s_type_poi_pop/poi_pop_all >s_require 的定义为敏感语义
*/

#include "../edge_cluster_graph.h"
#include "../util.h"
#include <ctime>
#include <cstdlib>
#include <vector>
#include <set>
#include <map>
using namespace std;

class Lppa_cfso
{
public:
	Lppa_cfso(EC_Graph *p_graph)
	{
		this->p_graph = p_graph;
		anonymization_time_total = 0;
		cnt_of_failure = 0;
		cnt_of_success = 0;
		l_max = 30;
        query_cnt = 100000;
		//打印当前地图状态
		srand((unsigned)time(NULL));
	}
	~Lppa_cfso()
	{
		delete[]is_node_selecteds;
		delete[]is_node_candidate;
	}
	//保护算法
	void lpp()
	{
		const vector<EC_Node*> &nodes = p_graph->get_nodes();
		LBS_User *pu = nullptr;
		clock_t start, finish;
		is_node_selecteds = new bool[nodes.size()];
		is_node_candidate = new bool[nodes.size()];
		start = clock();
        
		for (int i = 0; i < nodes.size(); i++) {
			for (int j = 0; j < nodes[i]->get_users().size(); j++) {
				pu = nodes[i]->get_users()[j];
				users.push_back(nodes[i]->get_users()[j]);
				vv_cloak_sets.push_back(ec_sae(pu, nodes[i]));
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
		vector<Edge*> adj_edges,cloak_set;
		vector<Edge*>::iterator it_cloak_set;
		Node *pn1, *pn2;
		int all_openvetex_cnt = 0, all_edge_size = 0;

		int user_cnt = vv_cloak_sets.size();
		for (int i = 0; i < vv_cloak_sets.size(); i++) {
			cloak_set.clear();
			for (int j = 0; j < vv_cloak_sets[i].size(); j++)
				cloak_set.push_back(vv_cloak_sets[i][j]->get_src_edge());
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
	vector<EC_Node*> ec_sae(LBS_User *&pu, EC_Node *pn)
	{
		k = 0;
		l = 0;
		accumulate_pop = 0.0;
		accumulate_svalue = 0.0;
		memset(is_node_selecteds, false, p_graph->get_nodes().size());
		memset(is_node_candidate, false, p_graph->get_nodes().size());
		double w2 = 0.1; // w2调节结构的重要程度，匿名服务器设置，会有参数讨论
		is_success = false;
		double accumulate_svalue = 0, accumulate_pop = 0;
		vector<EC_Node*> cloak_set;
		map<EC_Node*, pair<double, double>> candidate_map;
		const vector<double> &sensitive_vals = pu->get_sensitive_vals();
		is_success = add_node_to_cloakset(pu, pn, cloak_set, candidate_map);
		for (int i = 1; i < l_max && !is_success; i++) {
			map<EC_Node*, pair<double, double>>::iterator it_candidate, it_maximal_score;
			double k_score = 0, s_score = 0, struct_score = 0;
			if (candidate_map.size() < 1) break;
            
			double minimal_miss_s = numeric_limits<double>::max(), maximal_miss_s = numeric_limits<double>::min(), maximal_score = 0;
			//--
			for (it_candidate = candidate_map.begin(); it_candidate != candidate_map.end(); it_candidate++) { //已经计算了的就不用计算
				double candidate_svalue = it_candidate->second.first;
				double candidate_pop = it_candidate->second.second;
				double missed_value = candidate_svalue / pu->get_s() - candidate_pop;
				if (missed_value < minimal_miss_s) minimal_miss_s = missed_value;
				if (missed_value > maximal_miss_s) maximal_miss_s = missed_value;
			}
			for (it_candidate = candidate_map.begin(); it_candidate != candidate_map.end(); it_candidate++) { //已经计算了的就不用计算

				if (k >= pu->get_k()) {
					k_score = 1;
				}
				else {
					k_score = ((double)k + (it_candidate->first->get_users().size())) / pu->get_k();
					k_score = k_score < 1 ? k_score : 1;
				}
				if (fabs(maximal_miss_s - minimal_miss_s) < 0.001) {
					s_score = 1;
				}
				else {
					double candidate_svalue = it_candidate->second.first;
					double candidate_pop = it_candidate->second.second;
					double missed_value = candidate_svalue / pu->get_s() - candidate_pop;
					s_score = (maximal_miss_s - missed_value) / (maximal_miss_s - minimal_miss_s);
				}
				//struct
				vector<EC_Node*> &this_adj_nodes = it_candidate->first->get_adj_nodes();
				int cnt_of_inner_node = 0;

				//性能瓶颈
				for (int i = 0; i < this_adj_nodes.size(); i++) {
					if (is_node_selecteds[this_adj_nodes[i]->get_id()]) {
						cnt_of_inner_node++;
					}
					if (cnt_of_inner_node > 1) break;
				}

				////openvetex是评价指标
				if (cnt_of_inner_node > 1) { //边图成环
					struct_score = 1;

				}
				else {
					struct_score = 0;
				}
				//struct_score = 0;
				//struct_score = (double)rand() / RAND_MAX *0.3;
				double score = (1 - w2) *k_score * s_score + w2*struct_score;
				//选择得分最高的
				if (score > maximal_score) {
					maximal_score = score;
					it_maximal_score = it_candidate;
				}
			}

			is_success = add_node_to_cloakset(pu, it_maximal_score->first, cloak_set, candidate_map);
			is_node_candidate[it_maximal_score->first->get_id()] = false;
			candidate_map.erase(it_maximal_score);

		}//end for
		return cloak_set;
	}


	bool add_node_to_cloakset(LBS_User *&pu, EC_Node *new_node, vector<EC_Node*> &cloak_set, map<EC_Node*, pair<double, double>> &candidate_map)
	{
		//更新匿名集和候选集
		const vector<double> &sensitive_vals = pu->get_sensitive_vals();
		cloak_set.push_back(new_node);
		is_node_selecteds[new_node->get_id()] = true;
		Node *pn1, *pn2;
		vector<EC_Node*> &adj_nodes = new_node->get_adj_nodes(); //只影响刚加入的部分

		for (int i = 0; i < adj_nodes.size(); i++) {
			if (!is_node_candidate[adj_nodes[i]->get_id()] && !is_node_selecteds[adj_nodes[i]->get_id()]) { //加入候选集
				double candidate_edge_svalue = 0.0, candidate_edge_pop = 0.0;
				const vector<Poi*> &e_pois = adj_nodes[i]->get_pois();
				for (int j = 0; j < e_pois.size(); j++) { //多个兴趣点
					int poi_type = (int)e_pois[j]->get_type();
					candidate_edge_svalue += (sensitive_vals[poi_type] * e_pois[j]->get_pop());
					candidate_edge_pop += e_pois[j]->get_pop();
				}
				candidate_map[adj_nodes[i]] = make_pair(candidate_edge_svalue, candidate_edge_pop);
				is_node_candidate[adj_nodes[i]->get_id()] = true;
			}
		}
		//计算当前状态
		k += new_node->get_users().size();
		l++;
		double candidate_svalue = 0.0, candidate_pop = 0.0;
		if (!is_node_candidate[new_node->get_id()]) { //第一次执行需要再计算边上的兴趣点
			const vector<Poi*> &e_pois = new_node->get_pois();
			for (int j = 0; j < e_pois.size(); j++) { //多个兴趣点
				int poi_type = (int)e_pois[j]->get_type();
				candidate_svalue += (sensitive_vals[poi_type] * e_pois[j]->get_pop());
				candidate_pop += e_pois[j]->get_pop();
			}
		}
		else {
			candidate_svalue = candidate_map[new_node].first;
			candidate_pop = candidate_map[new_node].second;
		}
		accumulate_svalue += candidate_svalue;
		accumulate_pop += candidate_pop;
		if (k < pu->get_k() || l < pu->get_l() || (accumulate_pop > 0.001 && accumulate_svalue / accumulate_pop > pu->get_s())) return false;
		return true;
	}
	vector<LBS_User*> get_users()
	{
		return users;
	}
	vector<vector<EC_Node*>> get_vv_cloak()
	{
		return vv_cloak_sets;
	}
	vector<bool> get_v_success()
	{
		return v_success;
	}
private:
	//每个匿名用户
	int k, l;
	bool *is_node_selecteds; //hash set插入速度好慢
	bool *is_node_candidate; 
	bool is_success;
	double accumulate_svalue, accumulate_pop;
	//统计信息
	vector<bool> v_success;
	vector<LBS_User*> users;
	vector<vector<EC_Node*>> vv_cloak_sets;
	double anonymization_time_total;
	int cnt_of_success;
	int cnt_of_failure;
	vector<int> open_vetex_cnt; //将用于以权值减去环数
	int l_max; //匿名集最多选择l_max条边,否则失败
    int query_cnt;
	EC_Graph *p_graph;
};


#endif
