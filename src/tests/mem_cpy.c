# include "../data/mem_cpy.h"
# include <time.h>
# include <stdio.h>
# include <string.h>

# define max 64*64

void test(mdl_uint_t __bc) {
	mdl_u8_t static src[max], dst[max];

	mdl_uint_t c = 200, itr = 0;
	mdl_uint_t ns = 0;

	while(itr != c) {
		struct timespec begin, end;
		clock_gettime(CLOCK_MONOTONIC, &begin);

		ffly_mem_cpy(dst, src, __bc);

		clock_gettime(CLOCK_MONOTONIC, &end);
		ns += end.tv_nsec-begin.tv_nsec;
		itr++;
	}

	printf("mem_ncpy, time taken: %lfns, bc: %u.\n", (double)ns/(double)c, __bc);
}

int main(void) {
	mdl_uint_t o = 1;
	while(o != max*2) {
		test(o);
		o*=2;
	}
}
