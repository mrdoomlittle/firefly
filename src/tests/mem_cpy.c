# include "../data/mem_cpy.h"
# include <time.h>
# include <stdio.h>
# include <string.h>
# include <mdlint.h>
# include "../system/errno.h"
# define max 64*64

void test(mdl_uint_t __bc) {
	mdl_u8_t static src[max], dst[max];

	mdl_uint_t c = 200, itr = 0;
	double time = 0.0;

	while(itr != c) {
		struct timespec begin, end;
		clock_gettime(CLOCK_MONOTONIC, &begin);

		ffly_mem_cpy(dst, src, __bc);

		clock_gettime(CLOCK_MONOTONIC, &end);
		time+= (end.tv_nsec-begin.tv_nsec)+((end.tv_sec-begin.tv_sec)*1000000000.0);
		itr++;
	}

	time /= (double)c;

	printf("mem_cpy, time taken: %lfns or %lfsec, bc: %u.\n", time, time/1000000000.0, __bc);
}
# include "../system/io.h"
int main(void) {
	ffly_io_init();
	mdl_uint_t o = 1;
	while(o != max*2) {
		test(o);
		o*=2;
	}
	ffly_io_closeup();
	return FFLY_SUCCESS;
}
