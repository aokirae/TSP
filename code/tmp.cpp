#include <bits/stdc++.h>

using namespace std;

class Agent {
public:

	int a = -1;

	Agent(){}
	Agent(int a){
		this->a = a;
	}
};

int main(int argc, char const *argv[]) {
	Agent x;
	Agent y = Agent(10);

	std::cout << "y:" << y.a << '\n';
	if (x.a == -1) {
		std::cout << "Yes" << '\n';
	}
	return 0;
}
