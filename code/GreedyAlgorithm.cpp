#ifndef GreedyAlgorithm
#define GreedyAlgorithm

#include <bits/stdc++.h>

using namespace std;

std::vector<int> calcGreedyAlgorithm(std::vector<std::vector<double>> matrix){
	// ret[経路する順番] = matrixの番号
	std::vector<int> ret(matrix.size());
	// passed[matrixの番号] = 通ったらfalse、通る前はtrue
	std::vector<bool> passed(matrix.size(),true);

	// 初期化
	ret[0] = 0;
	passed[0] = false;

	// 探索
	for (size_t ir = 1; ir < ret.size(); ir++) {
		int pre = ret[ir-1];

		pair<int, double> next = make_pair(-1, *max_element(matrix[pre].begin(), matrix[pre].end()));
		for (size_t im = 0; im < matrix[pre].size(); im++) {
			if (passed[im] && matrix[pre][im] < next.second) {
				next.first = im;
				next.second = matrix[pre][im];
			}
		}
		ret[ir] = next.first;
		passed[next.first] = false;
	}

	return ret;
}

#endif
