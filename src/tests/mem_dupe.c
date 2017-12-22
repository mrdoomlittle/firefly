# include "../data/mem_dupe.h"
# include "../memory/mem_free.h"
# include <time.h>
# include <mdlint.h>
# define max 64*64

void test(mdl_uint_t __bc) {
	mdl_uint_t c = 200, itr = 0;
    double time = 0.0;
	mdl_u8_t src[max];
	while(itr != c) {
		struct timespec begin, end;
		clock_gettime(CLOCK_MONOTONIC, &begin);

		void *p;
		ffly_mem_dupe((void const**)&p, src, __bc);

		clock_gettime(CLOCK_MONOTONIC, &end);
		__ffly_mem_free(p);
		time+= (end.tv_nsec-begin.tv_nsec)+((end.tv_sec-begin.tv_sec)*1000000000.0);
		itr++;
	}

	time /= (double)c;
	printf("mem_dupe, time taken: %lfns or %lfsec, bc: %u.\n", time, time/1000000000.0, __bc);
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
}
