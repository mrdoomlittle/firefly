# include "../data/mem_dupe.h"
# include "../memory/mem_free.h"
# include <time.h>
# define max 64*64

void test(mdl_uint_t __bc) {
	mdl_uint_t c = 200, itr = 0;
    mdl_uint_t ns = 0;

	mdl_u8_t src[max];
	while(itr != c) {
		struct timespec begin, end;
		clock_gettime(CLOCK_MONOTONIC, &begin);

		void *p;
		ffly_mem_dupe(&p, src, __bc);

		clock_gettime(CLOCK_MONOTONIC, &end);
		__ffly_mem_free(p);
		ns += end.tv_nsec-begin.tv_nsec;
		itr++;
	}

	printf("mem_dupe, time taken: %lfns, bc: %u.\n", (double)ns/(double)c, __bc);
}

int main(void) {
	mdl_uint_t o = 1;
	while(o != max*2) {
		test(o);
		o*=2;
	}
}
