# include <cstdio>
# include "cuda_helper.hpp"
# include <eint_t.hpp>
int main() {
	mdl::uint_t x;
	mdl::firefly::graphics::max_threads(x);
	printf("end. %d\n", x);
}
