# include "ffint.h"
# include "clock.h"
# include "system/sched.h"
# include "system/io.h"
# include "system/nanosleep.h"
/* small internal routines
* underground
*/

ff_i8_t static stop = -1;
/*
	not just for the clock.
*/
void static*
update(void *__arg_p) {
_again:
	// use timespec later for now this as theres nothing else within
	ffly_nanosleep(0, 1000000);
	ffly_sched_clock_tick(1);
	ffly_clock_tick();
	ffly_fprintf(ffly_log, "tick tock, clock: %u\n", clock);
	if (stop == -1)
		goto _again;
	return NULL;
}

# include "system/thread.h"
ff_tid_t static t;
void ffly_bog_start(void) {
	ffly_thread_create(&t, update, NULL);
}

void ffly_bog_stop(void) {
	stop = 0;
	ffly_thread_wait(t);
}
