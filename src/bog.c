# include "ffint.h"
# include "clock.h"
# include "system/sched.h"
# include "system/io.h"
# include "system/nanosleep.h"
# include "pulse.h"
# include "linux/time.h"
/* small internal routines
* underground/sub/watch
*/

# define HOLDUP_SEC 0
# define HOLDUP_NSEC 100000000
ff_i8_t static stop = -1;
/*
	not just for the clock.
*/

static struct timespec start = {0, 0}, end;
static ff_u64_t delay;
void static*
update(void *__arg_p) {
_again:
	clock_gettime(CLOCK_MONOTONIC, &end);
	delay = (end.tv_sec-start.tv_sec)*1000000000;
	delay+=end.tv_nsec-start.tv_nsec;

	clock_gettime(CLOCK_MONOTONIC, &start);
	/*
		bigger delay less accurate by less cpu usage
	*/
	ffly_nanosleep(HOLDUP_SEC, HOLDUP_NSEC);
	if (ffly_pulse != NULL)
		ffly_pulse(delay);
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
