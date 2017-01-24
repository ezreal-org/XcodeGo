#ifndef LPPA_UTIL_H
#define LPPA_UTIL_H

#include <vector>
#include <fstream>
#include <string>
using namespace std;

//template<typename T>
//bool vector_find(vector<T> &v_input,T &value)
//{
//    vector<T>::iterator it_v;
//    for (it_v = v_input.begin(); it_v != v_input.end(); it_v++) {
//        if (*it_v == value) {
//            break;
//        }
//    }
//    if (it_v == v_input.end()) return false;
//    return true;
//}

//bool vv_find(vector<EC_Node*> &v_input, EC_Node* value)
//{
//    vector<EC_Node *>::iterator it_v;
//    for (it_v = v_input.begin(); it_v != v_input.end(); it_v++) {
//        if (*it_v == value) {
//            break;
//        }
//    }
//    if (it_v == v_input.end()) return false;
//    return true;
//}

//地图配置文件解析
class Graph_Config
{
public:
	//其实可以通过回溯递归的方法，传入一个配置文件，返回一个整个配置文件的map
	Graph_Config(string path)
	{
		ifstream config_file;
		config_file.open(path);
		if (config_file.bad()) {
			cout << "文件打开失败" << endl;
			return;
		}
		while (!config_file.eof()) {
			string line;
			getline(config_file, line);
			file_content.push_back(line);
		}
		config_file.close();
	}
	map<string, pair<vector<double>, int>>& parse_user_section()
	{
		string section_str = "LBS_USER";
		bool section_start = false;
		for (int i = 0; i < file_content.size(); i++) {
			if (file_content[i].size() < 2) continue;
			string this_line = file_content[i].substr(file_content[i].find_first_not_of(' '));
			if (this_line[0] == '#') continue;
			if (this_line.find(section_str) != -1) {
				section_start = true;
				continue;
			}
			if (!section_start) continue;
			if (this_line[0] == '[') break;
			//转换成k-v对
			int index_of_equal = this_line.find('=');
			string u_type = this_line.substr(0, index_of_equal);
			this_line = this_line.substr(index_of_equal + 1);
			int index_of_colon = this_line.find(':');
			string str_v = this_line.substr(1, index_of_colon-2);
			int cnt = std::atoi(this_line.substr(index_of_colon + 1).c_str());
			vector<double> profile;
			int index = str_v.find_first_of(',');
			int index_before = 0;
			while(index != std::string::npos)
			{
				profile.push_back(std::atof(str_v.substr(index_before,index-index_before).c_str()));
				index_before = index+1;
				index = str_v.find_first_of(',',index_before+1);
			}
			profile.push_back(std::atof(str_v.substr(index_before+1).c_str()));
			graph_users[u_type] = make_pair(profile, cnt);
		}
		return graph_users;
	}
	map<string, int>& parse_pois_section()
	{
		string section_str = "POI";
		bool section_start = false;
		for (int i = 0; i < file_content.size(); i++) {
			if (file_content[i].size() < 2) continue;
			string this_line = file_content[i].substr(file_content[i].find_first_not_of(' '));
			if (this_line[0] == '#') continue;
			if (this_line.find(section_str) != -1) {
				section_start = true;
				continue;
			}
			if (!section_start) continue;
			if (this_line[0] == '[') break;
			int index_of_equal = this_line.find('=');
			string key = this_line.substr(0, index_of_equal);
			int cnt = std::atoi(this_line.substr(index_of_equal+1).c_str());
			graph_pois[key]= cnt;
		}
		return graph_pois;
	}
private:
	vector<string> file_content;
	map<string, pair<vector<double>, int>> graph_users;
	map<string, int> graph_pois;
};
#endif
