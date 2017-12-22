# include "../data/find.h"
# include <string.h>
# include <stdio.h>
# include <time.h>
# include "../system/io.h"
# include <mdlint.h>
# include "../system/errno.h"
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
	double time = (end.tv_nsec-begin.tv_nsec)+((end.tv_sec-begin.tv_sec)*1000000000.0);
	printf("find, time taken: %lfns or %lfsec. itr_c: %lu, blk_size: %lu.\n", time, time/1000000000.0, res-a, sizeof(type));
	ffly_io_closeup();
	return FFLY_SUCCESS;
}
