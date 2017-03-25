# include "make_2darr.hpp"
# include <cstdio>
# include <boost/cstdint.hpp>
int main() {
	int * x = new int[4];
	boost::int8_t any_error = FFLY_SUCCESS;
	x[1] = 21299;
	int **y = mdl::firefly::data::make_2darr<int>(2, 2, x, 5, any_error, true);
	if (any_error != FFLY_SUCCESS) {

		return 0;
	}

	for (std::size_t o = 0; o != 4; o ++)
		printf("%d\n", x[o]);

	printf("---------------\n");

	for (std::size_t o = 0; o != 2; o ++)
		for (std::size_t r = 0; r != 2; r ++)
			printf("%d\n", y[o][r]);
	std::free(x);
	std::free(y);

}
