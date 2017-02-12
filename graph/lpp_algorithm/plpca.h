#ifndef LPPA_PLPCA_H
#define LPPA_PLPCA_H
#include "../edge_cluster_graph.h"
#include "../lbs_user.h"
#include "../poi.h"
#include "../util.h"
#include <vector>
#include <set>
#include <map>
using namespace std;

/*
	在边图中执行算法，广度优先的方法找一个环
	找环类似薛娇文章,树形结构
	假如攻击者不知道边图的转换和匿名算法，就是攻击者
	有地图的信息，每个路段的流量信息；攻击者获得匿名集
	后不能有效的执行攻击。作者使用边图来快速找到一条边周围的
	邻居边和隐藏以上两种信息。

	为方便比较做以下调整，假设攻击者知道匿名算法和边图的转换，
	广度优先进行遍历,进行 l_require < L < l_max 轮，取满足隐私要求
	的最小环
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
		//统计
		//这一块将不计入匿名时间，只是用于分析算法性能
		//开放节点处于运行时未决状态,只作为评价指标
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
			if (cloak_set.size() < 1) { //匿名失败,没有找到环，在统计中算l_max
				//all_edge_size += l_max;
				//all_openvetex_cnt += l_max;
				continue;
			}
			node_set.clear();
			//初始化相关顶点集
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
		cout << "average openvetex: " << (double)all_openvetex_cnt / cnt_of_success << endl;
		cout << "average cloak size: " << (double)all_edge_size / cnt_of_success << endl;

	}

	vector<EC_Node*> plpca(LBS_User *pu,EC_Node * const pn) //为该用户执行算法
	{
		map<pair<int,EC_Node*>, pair<int, EC_Node*>> pre_map; //为每个用户执行匿名算法时，记录前驱,int表示位置，EC_Node*表示某个节点
		vector<vector<EC_Node*>> vv_circles; //得到多个环
		vector<EC_Node*> expand_queue;
		vector<EC_Node*> v_cloak; //匿名集
		int lay_now,size_of_this_lay; // accumulate_expand_index 起到类似哈希的作用,同一个EC_Node它的前驱在不同层不同位置的前驱不一致;表明扩展时第几个处理到该顶点
		int accumulate_queue_index = 0, accumulate_expand_index = 0;
		expand_queue.push_back(pn);
		pre_map.clear();
		pre_map[make_pair(0,pn)] = make_pair(-1,pn); //起点前驱为空
		size_of_this_lay = 1;
		//获得所有环
		for (lay_now=0; lay_now < l_max; lay_now++) {
			int size_of_next_lay = 0;
			for (int i = 0; i < size_of_this_lay; i++) {
				EC_Node *node_now = expand_queue[accumulate_queue_index];
				vector<EC_Node*> &adj_nodes = node_now->get_adj_nodes();
				for (int j = 0; j < adj_nodes.size(); j++) {
					if (adj_nodes[j] == pn && lay_now>1) { //找到一个环,环起码有三条边
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
						//新增的顶点，为达到环的结构，应该在广度优先的某条扩展路径上不重复
						if (adj_nodes[j] == pn) continue; //这种为环只有两条边的情形，即通过第一层顶点就找到出发点
						pair<int, EC_Node*> p_node_info = pre_map[make_pair(accumulate_queue_index, node_now)];
						while (p_node_info.second != pn) { 
							if (p_node_info.second == adj_nodes[j])
								break;
							p_node_info = pre_map[p_node_info];
						}
						if (p_node_info.second == pn) {  //路径上不重复
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
		//确定满足要求的最小还
		int size_of_circle = vv_circles.size();
		//按边数排序
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
		//找最小满足要求的环
		vector<double> sensitive_vals = pu->get_sensitive_vals();
		is_success = false;
		for (int i = 0; i < size_of_circle; i++) {
			int edge_size = vv_circles[i].size();
			if (edge_size < pu->get_l()) continue; //边数少于用户需求l
			double accumulate_svalue = 0.0, accumulate_pop = 0.0;
			int k_value = 0, l_value = edge_size;
			for (int j = 0; j < edge_size; j++) {
				Edge *p_edge = vv_circles[i][j]->get_src_edge();
				k_value += p_edge->get_users().size();
				vector<Poi*> e_pois = p_edge->get_pois();
				for (int m = 0; m < e_pois.size(); m++) { //多个兴趣点
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
	bool is_success; //记录该用户匿名是否成功
	vector<bool> v_success;
	vector<LBS_User*> users;
	//统计信息
	double anonymization_time_total;
	int cnt_of_success;
	int cnt_of_failure;
	vector<int> open_vetex_cnt; //将用于以权值减去环数
	int l_max; //匿名集最多选择l_max条边,否则失败
	EC_Graph *p_graph;
};
#endif
