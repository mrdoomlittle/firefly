# include "vector.hpp"
# include <cstdio>
using namespace mdl::firefly::system;
int main() {
	vector<int> my_vec;

	my_vec.resize(12);
	//my_vec[11] = 21;
	my_vec.push_back() = 21;
	printf("end %d.\n", *my_vec.end());
}
