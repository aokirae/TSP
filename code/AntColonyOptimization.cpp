#ifndef AntColonyOptimization
#define AntColonyOptimization

#include <bits/stdc++.h>

using namespace std;

class Agent {
public:
	// 値変数
	// 現在位置
	int current;
	// フェロモンの優先度
	int alpha = 1;
	// 距離の優先度
	int beta = 4;
	// 距離の定数
	// これいる？
	int Q = 100;
	// フェロモンの蒸発率
	double ro = 0.4;

	// 全地点
	std::vector<int> whole;
	// 未訪問
	std::vector<int> not_visited;
	// 訪問済み
	std::vector<int> visited;

	// 全地点間の距離
	std::vector<std::vector<double>> roads;
	// 全地点間のフェロモン蓄積量
	// もしかすると全個体の全地点のフェロモン蓄積量かもしれない
	// その場合は以下になる
	// std::vector<std::vector<std::vector<double>>> pheromone;
	std::vector<std::vector<double>> pheromone;
	// このエージェントがその道を使ってるかどうか
	std::vector<std::vector<bool>> way;

	Agent (){}
	// 基本的な初期化
	Agent (std::vector<int> towns, int start, std::vector<std::vector<double>> roads, std::vector<std::vector<double>> pheromone){
		whole = not_visited = towns;
		not_visited.erase(find(not_visited.begin(),not_visited.end(),start));
		visited.push_back(start);

		current = start;

		this->roads = roads;
		this->pheromone = pheromone;

		way.resize(whole.size(), std::vector<bool> (whole.size(), false));
	}

	// 歩かせる
	void agentWalk(){
		// 全地点の回数分まわす
		for (size_t iw = 0; iw < whole.size(); iw++) {
			std::vector<double> prob = probability();
			// 次に遷移する地点を決める
			int next_t = -1;
			//random_device rnd;
			double choice = (double)rand() / RAND_MAX;
			// std::cout << "choice : " << choice << '\n';
			std::cout << "prob size : " << prob.size() << '\n';
			for (size_t ip = 0; ip < prob.size(); ip++) {
				choice -= prob[ip];
				if (choice < 0.0) {
					// std::cout << "not_visited" << not_visited[ip] << '\n';
					next_t = not_visited[ip];
					break;
				}
			}
			// std::cout << "current : " << current << '\n';
			// std::cout << "next_t : " << next_t << '\n';
			way[current][next_t] = true;
			current = next_t;
			visited.push_back(next_t);
			not_visited.erase(find(not_visited.begin(),not_visited.end(),next_t));

			// ありえないはずだけど、一応終了条件を設ける
			// ありえないという表現は、本来全ての地点を回らないといけないため
			if (not_visited.size() == 0) {
				return ;
			}
		}
		return ;
	}

	// 訪問していない各都市の評価値
	double assessment(int j){
		double numerator = pow(pheromone[current][j],alpha) * pow(1.0 / roads[current][j], beta);
		double denominator = 0.0;
		for (size_t l = 0; l < not_visited.size(); l++) {
			denominator = pow(pheromone[current][not_visited[l]], alpha) * pow(1.0 / roads[current][not_visited[l]], beta);
		}

		return numerator / denominator;
	}

	// 訪問していない各都市への遷移確率
	std::vector<double> probability(){
		std::vector<double> p;
		// 分母
		double sum_asses = 0.0;
		for (size_t inv = 0; inv < not_visited.size(); inv++) {
			sum_asses += assessment(not_visited[inv]);
		}
		// 分子を計算しつつpにいれていく
		for (size_t inv = 0; inv < not_visited.size(); inv++) {
			p.push_back(assessment(not_visited[inv]) / sum_asses);
		}

		return p;
	}

	// 全地点間のフェロモン
	std::vector<std::vector<double>> getDeltaPheromon(){
		double sum_length = getLength();
		std::vector<std::vector<double>> delta_pheromon(whole.size(), std::vector<double> (whole.size(),0.0));
		for (size_t iw = 0; iw < way.size(); iw++) {
			for (size_t jw = 0; jw < way[iw].size(); jw++) {
				if (way[iw][jw]) {
					delta_pheromon[iw][jw] = Q / sum_length;
				}
				else {
					delta_pheromon[iw][jw] = 0.0;
				}
			}
		}

		return delta_pheromon;
	}

	// エージェントが作成した回路の長さの取得
	double getLength(){
		double sum_length = 0.0;
		for (size_t i = 0; i < visited.size()-1; i++) {
			sum_length += roads[visited[i]][visited[i+1]];
		}
		return sum_length;
	}

	// フェロモンの更新
	// 通常個体は使わない
	// bestAgentが外部変数とroを使う
	std::vector<std::vector<double>> getNextPheromon(std::vector<std::vector<double>> pre_pheromone, std::vector<std::vector<std::vector<double>>> pheros){

		std::vector<std::vector<double>> next_pheromone = pre_pheromone;

		for (size_t i = 0; i < next_pheromone.size(); i++) {
			for (size_t j = 0; j < next_pheromone[i].size(); j++) {
				next_pheromone[i][j] *= ro;
			}
		}
		for (size_t i = 0; i < pheros.size(); i++) {
			for (size_t j = 0; j < pheros[i].size(); j++) {
				for (size_t k = 0; k < pheros[i][j].size(); k++) {
					next_pheromone[j][k] += pheros[i][j][k];
				}
			}
		}

		return next_pheromone;
	}
};


std::vector<int> clacAntColonyOptimization(std::vector<std::vector<double>> matrix){
	// 街の数
	int num_of_town = matrix.size();
	// エージェント(蟻)の数
	int num_of_agent = 30;
	// 蟻を放つ回数
	int num_of_solve = 500;

	// 各街
	// 前から順に0始まりのidを振る
	std::vector<int> towns(num_of_town);
	for (size_t i = 0; i < towns.size(); i++) {
		towns[i] = i;
	}
	// 各路に蓄積しているフェロモン
	// とりあえず一様に振る
	std::vector<std::vector<double>> pheromone(num_of_town, std::vector<double> (num_of_town, 0.5));
	for (size_t i = 0; i < num_of_town; i++) {
		pheromone[i][i] = 0.0;
	}

	// 最短距離 : スコア
	double min_length = -1;
	// 最良個体
	Agent best_agent;
	// フェロモンの収束判定を行うための変数
	double last_pheromone = 0.0;

	for (size_t inos = 0; inos < num_of_solve; inos++) {
		std::cout << "試行回数 : " << inos << '\n';
		// 全個体のフェロモン
		std::vector<std::vector<std::vector<double>>> pheros;
		for (size_t inoa = 0; inoa < num_of_agent; inoa++) {
			std::cout << "蟻" << inoa << "匹目" << '\n';
			Agent k = Agent(towns,0,matrix,pheromone);
			std::cout << "walking………" << '\n';
			k.agentWalk();
			std::cout << "end walking" << '\n';
			double length = k.getLength();
			pheros.push_back(k.getDeltaPheromon());

			// 最良個体は保存
			if (min_length == -1 || min_length > length) {
				min_length = length;
				best_agent = k;
			}
		}

		// フェロモンの更新
		std::cout << "フェロモンの更新" << '\n';
		pheromone = best_agent.getNextPheromon(pheromone, pheros);

		// フェロモンの更新が止まったとき、フェロモンをランダムで振る
		double sum_pheromone = 0.0;
		for (size_t i = 0; i < pheromone.size(); i++) {
			sum_pheromone += accumulate(pheromone[i].begin(),pheromone[i].end(), 0.0);
		}
		if (round(sum_pheromone * pow(10,5))/pow(10,5) == round(last_pheromone * pow(10,5))/pow(10,5)) {
			random_device rnd;
			mt19937 mt(rnd());
			for (size_t i = 0; i < pheromone.size(); i++) {
				for (size_t j = 0; j < pheromone[i].size(); j++) {
					if (i == j) {
						pheromone[i][j] = 0.0;
					}
					else {
						pheromone[i][j] = mt();
					}
				}
			}
		}
		last_pheromone = sum_pheromone;
	}


	return best_agent.visited;
}

#endif
