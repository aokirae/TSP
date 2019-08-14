#ifndef TwoOptAlgorithm
#define TwoOptAlgorithm

#include <bits/stdc++.h>

#include "GreedyAlgorithm.cpp"

using namespace std;

// x番目とy番目をいれかえる
// つまり
//		1. x番目までを保存
//		2. x+1番目からy番目までを逆順に保存
//		2. y+1番目からを保存
std::vector<int> twoOptSwap(std::vector<int> route, int x, int y){
	std::vector<int> new_route = route;

	// 逆順に保存
	for (int i = x+1; i < y+1; i++) {
		new_route[i] = route[y+x+1-i];
	}

	return new_route;
}


std::vector<int> calc2optAlgorithm(std::vector<std::vector<double>> matrix){
	// 回る順番
	// とりあえず貪欲で初期解を求める
	std::vector<int> route = calcGreedyAlgorithm(matrix);

	// 改善
	for (int improved = 1; improved;){
		improved = 0;
		for (size_t i = 0; i < route.size()-2; i++) {
			for (size_t j = i+2; j < route.size()-1; j++) {
				if (matrix[route[i]][route[j]] + matrix[route[i+1]][route[j+1]] < matrix[route[i]][route[i+1]] + matrix[route[j]][route[j+1]]) {
					route = twoOptSwap(route, i, j);
					improved = 1;
				}
			}
		}
	}

	return route;
}


#endif
