# define __ffly_debug_enabled
# include "types/err_t.h"
# include "dep/mem_cpy.h"
# include "system/thread.h"
# include "stdio.h"
# include "malloc.h"
# include "thread.h"
# include "mutex.h"

void *ptr = NULL;
mdl_i8_t go = -1;
__thread int test = 0;

void* _tr0(void *__arg_p) {
	test = 21;
	ffly_printf("thread 0\n");
	malloc(1);
}

void* _tr1(void *__arg_p) {
	ffly_printf("thread 1\n");
	malloc(1);
}

# include "system/nanosleep.h"
# include "linux/signal.h"
# include "linux/types.h"
# include "linux/sched.h"
# include "linux/unistd.h"
ffly_err_t ffmain(int __argc, char const *__argv[]) {
	ffly_tid_t tr0;
	ffly_tid_t tr1;
//	ffly_thread_create(&tr0, _tr0, NULL);
//	ffly_thread_create(&tr1, _tr1, NULL);

//	ffly_thread_wait(tr0);
//	ffly_thread_wait(tr1);

//	ffly_printf("%u\n", test);
//	ffly_printf("main.\n");

	ffly_arstat();
}
