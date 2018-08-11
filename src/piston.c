# include "piston.h"
# include "system/sched.h"
# include "system/nanosleep.h"
# include "system/thread.h"
# include "system/io.h"
# include "malloc.h"
// not finished and not tested

ff_tid_t static pistons[20];
ff_tid_t static *piston = pistons;

ff_i8_t static stall = -1;
void static*
handle(void *__arg_p) {
	ffly_printf("piston online.\n");
	while(stall == -1) {
		ffly_nanosleep(0, 50000000);

		ffly_scheduler_tick();
		ffly_fprintf(ffly_log, "piston tick\n");
	}
	ffly_printf("piston stall.\n");
}

void ffly_pistons_stall(void) {
	stall = 0;
	ff_tid_t *p = pistons;
	while(p != piston)
		ffly_thread_wait(*(p++));
}

ff_err_t ffly_piston(void) {
	ffly_fprintf(ffly_log, "piston creation.\n");
	ffly_thread_create(piston++, handle, NULL);
}
