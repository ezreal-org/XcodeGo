#ifndef REPLAY_ATTACK
#define REPLAY_ATTACK
#include "ec_sae.h"
#include "plpca.h"
#include "sa.h"
#include <vector>
#include <cmath>
#include "../lbs_user.h"
#include "../edge_cluster_graph.h"
#include "../util.h"
/*
攻击者知道用户在一个匿名集s中，知道匿名算法，并通过背景知识知道用户在路网的分布
知道用户的隐私偏好(k,l,s,v),攻击者想定位用户真实位置所在的路网边
攻击者通过假定用户在某条边，然后执行匿名算法，得到一个匿名集s',通过比较两个匿名集的
相似性
*/
class Replay_attack
{
public:
	Replay_attack(EC_Graph *p_graph)
	{
		this->p_graph = p_graph;
		srand((unsigned)time(NULL));
        sa_attack();
        plpca_attack();
        ec_sae_attack();

	}
	void sa_attack()
	{
		Lppa_sa *p_sa = new Lppa_sa(this->p_graph->p_graph);
		p_sa->lpp();
		cnt_of_attack = 0;
		cnt_of_success = 0;
		cout << "executing replay attack:" << endl;
		vector<LBS_User*> users = p_sa->get_users();
		vector<bool> is_success = p_sa->get_v_success();
		vector<vector<Edge*>> vv_cloak_set = p_sa->get_vv_cloaks();

		for (int i = 0; i < users.size(); i++) { //对用户i的匿名集 vv_cloak[i]进行攻击
			double maximal_similarity = 0;
			vector<Edge*> maximal_edges; //最大相似度的可能有多条边
			if (!is_success[i]) continue; //不考虑未成功的匿名
			for (int j = 0; j < vv_cloak_set[i].size(); j++) {
				if (vv_cloak_set[i][j]->get_users().size() < 1) continue;
				vector<Edge*>sa_cs= p_sa->sa(users[i], vv_cloak_set[i][j]); //假定在用户在匿名集的某条边上
				double set_similarity = calculate_set_similarity(vv_cloak_set[i], sa_cs);
				if (fabs(set_similarity - maximal_similarity) < 0.001) {
					maximal_edges.push_back(vv_cloak_set[i][j]);
				}
				else if (set_similarity > maximal_similarity) {
					maximal_similarity = set_similarity;
					maximal_edges.clear();
					maximal_edges.push_back(vv_cloak_set[i][j]);
				}
			}
			int selected_index = rand() % maximal_edges.size();
			vector<LBS_User*> users_in_edge = maximal_edges[selected_index]->get_users();
			cnt_of_attack++;
			if (vector_find(users_in_edge, users[i])) { //用户确实在此边上，攻击成功
				cnt_of_success++;
			}
		}
        cout << "attack nums:" << cnt_of_attack << " success nums：" << cnt_of_success << endl;
		cout << "------" << endl;
		delete p_sa;
	}
	void plpca_attack()
	{
		Lppa_plpca *p_sa = new Lppa_plpca(this->p_graph);
		p_sa->lpp();
		cnt_of_attack = 0;
		cnt_of_success = 0;
		cout << "executing replay attack:" << endl;
		vector<LBS_User*> users = p_sa->get_users();
		vector<bool> is_success = p_sa->get_v_success();
		vector<vector<EC_Node*>> vv_cloak_set = p_sa->get_vv_cloak();

		for (int i = 0; i < users.size(); i++) { //对用户i的匿名集 vv_cloak[i]进行攻击
			double maximal_similarity = 0;
			vector<EC_Node*> maximal_edges; //最大相似度的可能有多条边
			if (!is_success[i]) continue; //不考虑未成功的匿名
			for (int j = 0; j < vv_cloak_set[i].size(); j++) {
				if (vv_cloak_set[i][j]->get_users().size() < 1) continue;
				vector<EC_Node*>sa_cs = p_sa->plpca(users[i], vv_cloak_set[i][j]); //假定在用户在匿名集的某条边上
				double set_similarity = calculate_set_similarity(vv_cloak_set[i], sa_cs);
				if (fabs(set_similarity - maximal_similarity) < 0.001) {
					maximal_edges.push_back(vv_cloak_set[i][j]);
				}
				else if (set_similarity > maximal_similarity) {
					maximal_similarity = set_similarity;
					maximal_edges.clear();
					maximal_edges.push_back(vv_cloak_set[i][j]);
				}
			}
			int selected_index = rand() % maximal_edges.size();
			vector<LBS_User*> users_in_edge = maximal_edges[selected_index]->get_users();
			cnt_of_attack++;
			if (vector_find(users_in_edge, users[i])) { //用户确实在此边上，攻击成功
				cnt_of_success++;
			}
		}
        cout << "attack nums:" << cnt_of_attack << "  success:" << cnt_of_success << endl;
		cout << "------" << endl;
		delete p_sa;
	}
	void ec_sae_attack()
	{
		Lppa_ecsa_e *p_sa = new Lppa_ecsa_e(this->p_graph);
		p_sa->lpp();
		cnt_of_attack = 0;
		cnt_of_success = 0;
		cout << "executing replay attack:" << endl;
		vector<LBS_User*> users = p_sa->get_users();
		vector<bool> is_success = p_sa->get_v_success();
		vector<vector<EC_Node*>> vv_cloak_set = p_sa->get_vv_cloak();

		for (int i = 0; i < users.size(); i++) { //对用户i的匿名集 vv_cloak[i]进行攻击
			double maximal_similarity = 0;
			vector<EC_Node*> maximal_edges; //最大相似度的可能有多条边
			if (!is_success[i]) continue; //不考虑未成功的匿名
			for (int j = 0; j < vv_cloak_set[i].size(); j++) {
				if (vv_cloak_set[i][j]->get_users().size() < 1) continue;
				vector<EC_Node*>sa_cs = p_sa->ec_sae(users[i], vv_cloak_set[i][j]); //假定在用户在匿名集的某条边上
				double set_similarity = calculate_set_similarity(vv_cloak_set[i], sa_cs);
				if (fabs(set_similarity - maximal_similarity) < 0.001) {
					maximal_edges.push_back(vv_cloak_set[i][j]);
				}
				else if (set_similarity > maximal_similarity) {
					maximal_similarity = set_similarity;
					maximal_edges.clear();
					maximal_edges.push_back(vv_cloak_set[i][j]);
				}
			}
			int selected_index = rand() % maximal_edges.size();
			vector<LBS_User*> users_in_edge = maximal_edges[selected_index]->get_users();
			cnt_of_attack++;
			if (vector_find(users_in_edge, users[i])) { //用户确实在此边上，攻击成功
				cnt_of_success++;
			}
		}
        cout << "attack nums:" << cnt_of_attack << " success nums:" << cnt_of_success << endl;
		cout << "------" << endl;
		delete p_sa;
	}
	template<typename T>
	double calculate_set_similarity(vector<T*> &cs, vector<T*> &ra_cs)
	{
		int size_union = cs.size();
		int size_intersection = 0;
		for (int i = 0; i < ra_cs.size(); i++) {
			if (vector_find(cs, ra_cs[i])) {
				size_intersection++;
			}
			else { //出现新边
				size_union++;
			}
		}
		return (double)size_intersection / size_union;
	}
private:
	EC_Graph *p_graph;
	double cnt_of_success; //成功的次数
	double cnt_of_attack; //攻击的次数
};
#endif
