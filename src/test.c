# define __ffly_debug_enabled
# include "types/err_t.h"
# include "dep/mem_cpy.h"
# include "system/thread.h"
# include "stdio.h"
# include "malloc.h"
# include "thread.h"
# include "mutex.h"
# include "system/nanosleep.h"
/*
	only for testing allocr need more woking on
*/
void *p = NULL;

mdl_i8_t r0 = -1, r1 = -1;
# define NUM 20
void* _tr0(void *__arg_p) {
	ffly_printf("thread 0\n");

//	p = malloc(1);
	r1 = 0;
	while(r0<0) {
		free(malloc(1));
	}
}

void* _tr1(void *__arg_p) {
	ffly_printf("thread 1\n");
	while(r1<0);
	ffly_nanosleep(1,0);
	mdl_uint_t i = 0;
	while(i++ != NUM) {
		free(malloc(1));
	}

	r0 = 0;
}


# include "system/nanosleep.h"
# include "linux/signal.h"
# include "linux/types.h"
# include "linux/sched.h"
# include "linux/unistd.h"
ffly_err_t ffmain(int __argc, char const *__argv[]) {
	ffly_tid_t tr0, tr1;
	ffly_thread_create(&tr0, _tr0, NULL);
	ffly_thread_create(&tr1, _tr1, NULL);
	ffly_thread_wait(tr0);
	ffly_thread_wait(tr1);

//	ffly_printf("%u\n", test);
//	ffly_printf("main.\n");

	ffly_arstat();
//	pr();
//	pf();
}
