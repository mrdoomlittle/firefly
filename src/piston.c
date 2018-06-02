# include "piston.h"
# include "system/sched.h"
# include "system/nanosleep.h"
# include "system/thread.h"
# include "system/io.h"
// not finished and not tested

ff_tid_t static pistons[20];
ff_tid_t static *piston = pistons;

ff_i8_t static stall = -1;
void static*
handle(void *__arg_p) {
	while(stall == -1) {
		ffly_printf("piston tick.\n");
		ffly_nanosleep(0, 100000000);
		ffly_scheduler_tick();
		ffly_sched_clock_tick(1);
	}
}

void ffly_pistons_stall(void) {
	stall = 0;
	ff_tid_t *p = pistons;
	while(p != piston)
		ffly_thread_wait(*(p++));
}

ff_err_t ffly_piston(void) {
	ffly_thread_create(piston++, handle, NULL);
}
