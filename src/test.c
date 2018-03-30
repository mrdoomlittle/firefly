# define __ffly_debug_enabled
# include "types/err_t.h"
# include "dep/mem_cpy.h"
# include "system/thread.h"
# include "stdio.h"
# include "malloc.h"
# include "thread.h"
# include "mutex.h"
# include "system/nanosleep.h"
# include "ctl.h"
/*
	only for testing allocr need more woking on
*/
ffly_potp pot;
void *p = NULL;

void *thr(void *__arg_p) {
	free(malloc(200));
	free(p);
	ffly_ctl(ffly_malc, _ar_setpot, (mdl_u64_t)pot);
	//free(malloc(200));
	
	ffly_ctl(ffly_malc, _ar_unset, 0);
}


# include "system/nanosleep.h"
# include "linux/signal.h"
# include "linux/types.h"
# include "linux/sched.h"
# include "linux/unistd.h"
ffly_err_t ffmain(int __argc, char const *__argv[]) {
	p = malloc(200);
	ffly_ctl(ffly_malc, _ar_getpot, (mdl_u64_t)&pot);
	ffly_tid_t id;
	ffly_thread_create(&id, thr, NULL);
	ffly_thread_wait(id);

	ffly_arstat();
}
