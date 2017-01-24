//
// Created by jayce on 23/11/2016.
//

#ifndef C_PLUS_PLUS_PRACTICE_WEIGHT_RANDOM_GENERATOR_H
#define C_PLUS_PLUS_PRACTICE_WEIGHT_RANDOM_GENERATOR_H

#include <ctime>
#include <cstdlib>
#include <vector>
using namespace std;

class Bignum_Generator
{
public:
	Bignum_Generator()
	{
		random_max = 0x7fff * 0x7fff; //linux,win两个平台定义不一样,防止越界
		srand(time(NULL));
	}
	int get_next_r()
	{
		return (rand() % 0x7fff)* (0x7fff) + (rand() % 0x7fff);
	}
	int get_random_max()
	{
		return random_max;
	}
private:
	int random_max; // 就支持int吧
};

//支持权值得生成器
class WRandom_Generator :public Bignum_Generator
{
public:
	WRandom_Generator(const vector<int> &keys_to_generate, const vector<int> &weight_vals)
	{
		random_range = 0; //真正产生随机数区间，accumulate_index维持一个映射到key
		accumulate_index.push_back(0);
		this->keys_to_generate = keys_to_generate;
		for (int i = 0; i<weight_vals.size(); i++) {
			random_range += weight_vals[i];
			accumulate_index.push_back(random_range);
		}
	}
	int get_next_wr()
	{
		int ret_val = get_next_r() % random_range;
		for (int i = accumulate_index.size() - 2; i >= 0; i--) {
			if (ret_val >= accumulate_index[i]) {
				return keys_to_generate[i];
			}
		}
	}

private:
	int random_range;
	vector<int> keys_to_generate;
	vector<int> accumulate_index;
};
#endif //C_PLUS_PLUS_PRACTICE_WEIGHT_RANDOM_GENERATOR_H
