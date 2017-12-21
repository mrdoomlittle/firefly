# include "../data/find.h"
# include <string.h>
# include <stdio.h>
# include <time.h>
# include "../system/io.h"
# include <mdlint.h>
int main(void) {
	ffly_io_init();
	typedef mdl_u8_t type;
	type a[] = {'a','b','c','d','e','f','g','h','i','j','k','m','n','o','p','q','r','s','t','u','v','w','x','y','z'};
	mdl_uint_t len = sizeof(a)/sizeof(type);

	type val = 'q';
	type *res;
	struct timespec begin, end;
	clock_gettime(CLOCK_MONOTONIC, &begin);

	res = ffly_find(a, &val, sizeof(type), len);

	clock_gettime(CLOCK_MONOTONIC, &end);
	printf("find, time taken: %luns. itr_c: %lu, blk_size: %lu.\n", end.tv_nsec-begin.tv_nsec, res-a, sizeof(type));
	ffly_io_closeup();
}
