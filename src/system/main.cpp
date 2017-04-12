# include "vector.hpp"
# include <cstdio>
# include <cstdio>
# include <queue>
using namespace mdl::firefly::system;
int main() {
	struct hi {
		hi() : a(){
			printf("%d\n", this-> x);
		}
		int x = 21;
		std::queue<int> a;
	};

	vector<hi> my_vec;

	my_vec.resize(3);
	new (my_vec.begin() - 1) hi();
	printf("%d\n", my_vec[1].a.size());
}
