#ifndef SimulatedAneealing
#define SimulatedAneealing

#include <bits/stdc++.h>

#include "GreedyAlgorithm.cpp"
#include "2optAlgorithm.cpp"

using ll = long long;

using namespace std;

// 総距離
double allDistance(std::vector<int> route,  std::vector<std::vector<double>> matrix){
	double sum = 0.0;
	for (size_t i = 0; i < route.size()-1; i++) {
		sum += matrix[route[i]][route[i+1]];
	}
	return sum;
}

std::vector<int> calcNextState(std::vector<int> route, std::vector<std::vector<double>> matrix){
	random_device rnd;
	int x = rnd() % route.size();
	std::vector<double> v = matrix[x];
	sort(v.begin(), v.end());
	auto itr = find(matrix[x].begin(), matrix[x].end(), v[rnd()%10]);
	int y = itr - matrix[x].begin();
	return twoOptSwap(route, x, y);
}

std::vector<int> calcSimulatedAnnealing(std::vector<std::vector<double>> matrix){
	// 乱数生成器
	random_device rnd;
	mt19937 mt(rnd());

	// とりあえず貪欲法でルートを求める
	std::vector<int> route = calcGreedyAlgorithm(matrix);

	// ベストの状態,ベストスコア,現在のスコア
	std::vector<int> best_route = route;
	double best_score = allDistance(route, matrix);
	double current_score = best_score;

	// この焼きなましにかける時間
	// 今回は30秒
	const ll use_time = 600;
	// 焼きなまし開始時間
	const ll sa_time_start = time(NULL);
	// 焼きなまし終了時間
	const ll sa_time_end = sa_time_start + use_time;
	// 現在時刻
	ll sa_time_current = sa_time_start;

	// loop回数
	ll loop = 0;
	// 焼きなまし終了時間までループ
	while ((sa_time_current = time(NULL) < sa_time_end)) {
		// 次の状態を求める
		std::vector<int> next_route = calcNextState(route, matrix);
		// 状態のスコア
		double next_score = allDistance(next_route, matrix);

		// 焼きなまし開始からの時間
		const ll t = sa_time_current - sa_time_start;
		const ll R = 10000;
		// スコアが悪くなった時でも、次の状態に遷移する場合はtrue
		const bool force_next = R * (use_time - t) > use_time * (mt()%R);

		// スコアが良くなった or 強制遷移
		if (next_score < current_score || force_next) {
			current_score = next_score;
			route = next_route;
		}

		// ベストスコアは残しておく
		if (current_score < best_score) {
			best_score = current_score;
			best_route = route;
		}

		loop++;
		if (loop%100 == 0) {
			std::cout << "loop:" << loop << '\n';
		}

	}

	return best_route;

}

#endif
