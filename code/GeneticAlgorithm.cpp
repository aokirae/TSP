#ifndef GeneticAlgorithm
#define GeneticAlgorithm

#include <bits/stdc++.h>

using namespace std;

double calcEvaluation(std::vector<std::vector<double>> matrix, std::vector<int> narabi){
	double sum = 0;
	for (size_t i = 0; i < narabi.size() - 1; i++) {
		sum += matrix[narabi[i]][narabi[i+1]];
	}
	return sum;
}

// 子を作る
// 1. 親1から順番を変えるところをランダムで一つ選ぶ = place0
// 2. 各親にて、親1[place0]の数字と親2[place0]の数字を入れ替えたものを子とする
std::vector<std::vector<int>> crossFamily(std::vector<int> parent1, std::vector<int> parent2){
	random_device rnd;
	int place0 = rnd() % parent1.size();
	int place1 = (int)(find(parent1.begin(), parent1.end(), parent2[place0]) - parent1.begin());
	int place2 = (int)(find(parent2.begin(), parent2.end(), parent1[place0]) - parent2.begin());
	std::vector<int> child1 = parent1;
	std::vector<int> child2 = parent2;
	child1[place0] = parent1[place1];
	child1[place1] = parent1[place0];
	child2[place0] = parent2[place2];
	child2[place2] = parent2[place0];

	return std::vector<std::vector<int>> {parent1,parent2,child1,child2};
}

// 自然淘汰
// MGGでいいでしょ
std::vector<int> naturalSelection(std::vector<pair<std::vector<int>, double>> family){
	// 最優個体とランダムで1個体を選ぶ
	random_device rnd;
	int choice0 = rnd() % family.size();
	int choice1 = 0;
	int m = family[0].second+1;
	for (int i = 0; i < family.size(); i++) {
		if (i == choice0) {
			continue;
		}
		if (m > family[i].second) {
			m = family[i].second;
			choice1 = i;
		}
	}
	return std::vector<int> {choice0, choice1};
}

// 突然変異
// 個体を確率で選び、2点入れ替える
std::vector<pair<std::vector<int>, double>> mutation(std::vector<pair<std::vector<int>, double>> individuals){
	// 乱数
	random_device rnd;
	mt19937 mt(rnd());
	std::uniform_int_distribution<> rand1000(0, 999);

	for (size_t i = 0; i < individuals.size(); i++) {
		// 0.5%で突然変異
		if (rand1000(mt) < 5) {
			std::vector<int> v = individuals[i].first;
			int place1 = mt() % v.size();
			int place2;
			do {
				place2 = mt() % v.size();
			} while(place1 == place2);
			// swap
			int tmp = v[place1];
			v[place1] = v[place2];
			v[place2] = tmp;
			// 保存
			individuals[i].first = v;
		}
	}
	return individuals;
}

void output(std::vector<pair<std::vector<int>, double>> better_invididuals){
	std::ofstream ofs("../data/遺伝的アルゴリズム途中経過.csv", std::ios::out);
	if (ofs.is_open()) {
		ofs << "評価値,並び順\n";
		for (size_t i = 0; i < better_invididuals.size(); i++) {
			ofs << better_invididuals[i].second;
			std::vector<int> v = better_invididuals[i].first;
			for (size_t iv = 0; iv < v.size(); iv++) {
				ofs << "," << v[iv];
			}
			ofs << "\n";
		}
		ofs.close();
	}
}


std::vector<int> calcGeneticAlgorithm(std::vector<std::vector<double>> matrix){
	// 個体数
	int individuals = 100;
	// 各世代の最優個体を確保しとく
	std::vector<pair<std::vector<int>, double>> better_invididuals;
	// 今世代の皆さん
	std::vector<pair<std::vector<int>, double>> now_individuals;
	// 今世代の最優個体
	std::pair<std::vector<int>, double> best_individual;

	// 乱数
	random_device rnd;
	mt19937 mt(rnd());
	uniform_int_distribution<> rand100(0, 99);
	uniform_int_distribution<> randindi(0, individuals-1);

	// 初期化
	std::vector<int> v(matrix.size());
	iota(v.begin(),v.end(), 1);
	int m = -1;
	do {
		if ((int)now_individuals.size() == individuals) {
			break;
		}
		if (rand100(mt) == 0) {
			double eva = calcEvaluation(matrix,v);
			if (m == -1 || m > eva) {
				best_individual = make_pair(v,eva);
			}
			now_individuals.push_back(make_pair(v, eva));
		}
	} while (next_permutation(v.begin(), v.end()));

	// 保存
	better_invididuals.push_back(best_individual);

	// まわしていくぅ
	// 収束条件は1000世代更新がなかったらにする
	int count = 0;
	int pre_evaluation = -1;
	while(count < 1000){
		// 親を2つ選ぶ
		int parent1 = randindi(mt);
		int parent2;
		do {
			parent2 = randindi(mt);
		} while(parent1 == parent2);

		// 家族を作る
		std::vector<std::vector<int>> fam = crossFamily(now_individuals[parent1].first, now_individuals[parent2].first);
		std::vector<pair<std::vector<int>, double>> family;
		for (size_t i = 0; i < fam.size(); i++) {
			double eva = calcEvaluation(matrix, fam[i]);
			if (eva < best_individual.second) {
				best_individual = make_pair(fam[i], eva);
			}
			family.push_back(make_pair(fam[i], eva));
		}

		// 自然淘汰
		// 2個体残るはず
		std::vector<int> ns = naturalSelection(family);
		now_individuals[parent1] = family[ns[0]];
		now_individuals[parent2] = family[ns[1]];

		// 突然変異
		now_individuals = mutation(now_individuals);

		// 保存
		better_invididuals.push_back(best_individual);

		// 終了条件更新
		if (pre_evaluation != best_individual.second) {
			pre_evaluation = best_individual.second;
			count = 0;
		}
		count++;
	}

	output(better_invididuals);

	return best_individual.first;
}

#endif
