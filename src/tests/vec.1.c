# include <time.h>
# include <stdio.h>
# include <string.h>
# include "../system/vec.h"
# include "../types/err_t.h"
# include "../system/errno.h"
# include "../system/err.h"
# include <mdlint.h>
# define failure {\
	err = FFLY_FAILURE;\
	goto _end;}
ffly_err_t err = FFLY_SUCCESS;
# include "../system/io.h"
# define NO_ELEMENTS 1238
int main(void) {
	ffly_io_init();
	struct ffly_vec vec;
	ffly_vec_clear_flags(&vec);
	ffly_vec_tog_flag(&vec, VEC_AUTO_RESIZE);
   // ffly_vec_tog_flag(&vec, VEC_NONCONTINUOUS);
	if (_err(ffly_vec_init(&vec, sizeof(mdl_u64_t)))) failure;

	double push_time = 0.0;
	double pop_time = 0.0;
	struct timespec begin, end;
	mdl_u64_t i = 0;
	while(i != NO_ELEMENTS) {
		mdl_u64_t *p;
		clock_gettime(CLOCK_MONOTONIC, &begin);
		if (_err(ffly_vec_push_back(&vec, (void**)&p))) failure;
		clock_gettime(CLOCK_MONOTONIC, &end);
		push_time+= (end.tv_nsec-begin.tv_nsec)+((end.tv_sec-begin.tv_sec)*1000000000.0);
		*p = i;
		i++;
	}

	while(i != 0) {
		mdl_u64_t v;
		clock_gettime(CLOCK_MONOTONIC, &begin);
		if (_err(ffly_vec_pop_back(&vec, &v))) failure;
		clock_gettime(CLOCK_MONOTONIC, &end);
		pop_time+= (end.tv_nsec-begin.tv_nsec)+((end.tv_sec-begin.tv_sec)*1000000000.0);
		if (v != --i) {
			printf("no match.\n");
			failure;
		}
	}

	push_time /= NO_ELEMENTS;
	pop_time /= NO_ELEMENTS;
	printf("push_time: [%lfns or %lfsec], pop_time: [%lfns or %lfsec]\n", push_time, push_time/1000000000.0, pop_time, pop_time/1000000000.0);
	_end:
	if (_err(ffly_vec_de_init(&vec)))
		err = FFLY_FAILURE;
	ffly_io_closeup();
	return err;
}
