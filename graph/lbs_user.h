#ifndef LBS_USER_H
#define LBS_USER_H
#include <vector>
using namespace std;

class LBS_User
{
public:
	LBS_User(long long id, int k, int l, double s_require, vector<double> s_values)
	{
		this->id = id;
		this->k_require = k;
		this->l_require = l;
		this->semantic_require = s_require;
		this->sensitive_vals = s_values;
	}
	long long get_uid()
	{
		return id;
	}
	int get_k()
	{
		return k_require;
	}
	int get_l()
	{
		return l_require;
	}
	double get_s()
	{
		return semantic_require;
	}
	const vector<double>& get_sensitive_vals()
	{
		return sensitive_vals;
	}
private:
	long long id;
	int k_require;
	int l_require;
	double semantic_require;
	vector<double> sensitive_vals;
};
#endif //LBS_USER_H
