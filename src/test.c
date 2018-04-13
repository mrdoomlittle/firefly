# define __ffly_debug_enabled
# include "types.h"
# include "dep/mem_cpy.h"
# include "system/thread.h"
# include "stdio.h"
# include "malloc.h"
# include "thread.h"
# include "mutex.h"
# include "system/nanosleep.h"
# include "ctl.h"
# include "string.h"
/*
	only for testing allocr need more woking on
*/
ffly_potp pot;
void *p = NULL;

void *thr(void *__arg_p) {
/*
	free(malloc(200));
	free(p);
	ffly_ctl(ffly_malc, _ar_setpot, (ff_u64_t)pot);
	//free(malloc(200));
	
	ffly_ctl(ffly_malc, _ar_unset, 0);
*/
ffly_printf("hello.\n");
}


# include "system/errno.h"
# include "system/nanosleep.h"
# include "linux/signal.h"
# include "linux/types.h"
# include "linux/sched.h"
# include "linux/unistd.h"
ff_i8_t run = -1;
void sig(int __no) {
	run = 0;
}
/*
void restore();
__asm__("restore:mov $15,%rax\n\t"
		"syscall");
*/
# include "signal.h"
# include "maths/round.h"
# include "maths/floor.h"
# include "maths/ceil.h"
# include "maths/sin.h"
# include "maths/cos.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
//	p = malloc(200);
//	ffly_ctl(ffly_malc, _ar_getpot, (ff_u64_t)&pot);
//	ffly_tid_t id;
//	ffly_thread_create(&id, thr, NULL);
//	ffly_thread_wait(id);
/*
	struct sigaction sa;
	memset(&sa, 0, sizeof(struct sigaction));
	sa.sa_handler = sig;
	sigaction(SIGINT, &sa, NULL);	
	while(run<0);
	ffly_printf("bye.\n");
*/
	double val = 1;
	double cos = 0, sin = 0; 
	cos = ffly_cos(val);
	sin = ffly_sin(val);

	ff_u64_t adr;
	__asm__("movq %%rsp, %0\n\t" : "=m"(adr));
	adr = (adr+(16-1))&-16;
	__asm__("movq %0, %%rsp\n\t" : : "m"(adr));
	ffly_printf("cos: %f, sin: %f\n", cos, sin);
	ffly_arstat();
}
