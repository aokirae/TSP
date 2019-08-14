#ifndef Main
#define Main

#include <bits/stdc++.h>

#include "GreedyAlgorithm.cpp"
#include "2optAlgorithm.cpp"
#include "SimulatedAnnealing.cpp"

using namespace std;


std::vector<std::string> split(const std::string &str, char sep){
    std::vector<std::string> v;
    std::stringstream ss(str);
    std::string buffer;
    while( std::getline(ss, buffer, sep) ) {
        v.push_back(buffer);
    }
    return v;
}

std::vector<std::vector<int>> input(int points){
	std::vector<std::vector<int>> ret;
	std::ifstream ifs("../data/edges/小日向美穂_エッジ選択_"+to_string(points)+"点.csv", std::ios::in);
	if (ifs.is_open()) {
		std::string line;
		while (std::getline(ifs, line)) {
			std::vector<string> str = split(line, ',');
			std::vector<int> v;
			v.push_back(stoi(str[0]));
			v.push_back(stoi(str[1]));
			ret.push_back(v);
		}
		ifs.close();
	}
	else {
		std::cerr << "Unable to open file\n";
	}

	return ret;
}

void output(string method, int points, std::vector<std::vector<int>> v){
	std::ofstream ofs("../data/solve/小日向美穂_"+method+"_"+to_string(points)+"点.csv", std::ios::out);
	if (ofs.is_open()) {
		for (size_t i = 0; i < v.size(); i++) {
			ofs << v[i][0] << ',' << v[i][1] << '\n';
		}
		ofs.close();
	}
}

std::vector<std::vector<int>> sortAnswer(std::vector<std::vector<int>> edges, std::vector<int> ret){
	std::vector<std::vector<int>> v;
	for (size_t i = 0; i < ret.size(); i++) {
		v.push_back(edges[ret[i]]);
	}
	return v;
}

// a点とb点の座標の距離を計算する
double calcEuclidDistance(std::vector<int> a, std::vector<int> b){
	return sqrt(pow(a[0]-b[0],2) + pow(a[1]-b[1], 2));
}

int main(int argc, char const *argv[]) {

	// 方法
	// string method = "貪欲法";
	// string method = "2opt法";
	string method = "焼きなまし法";

	// 点の個数
	int points = 3000;
	// 点の座標
	std::vector<std::vector<int>> edges = input(points);

	std::vector<std::vector<double>> matrix(edges.size(), std::vector<double> (edges.size()));
	for (size_t i = 0; i < edges.size(); i++) {
		for (size_t j = i+1; j < edges.size(); j++) {
			matrix[i][j] = matrix[j][i] = calcEuclidDistance(edges[i], edges[j]);
		}
	}

	std::vector<int> ret;
	if (method == "貪欲法") {
		ret = calcGreedyAlgorithm(matrix);
	}
	if (method == "2opt法") {
		ret = calc2optAlgorithm(matrix);
	}
	if (method == "焼きなまし法") {
		ret = calcSimulatedAnnealing(matrix);
	}


	output(method, points, sortAnswer(edges, ret));

	return 0;
}
#endif
