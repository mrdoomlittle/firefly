# include "ffint.h"
# include "ffly_def.h"
# include "system/io.h"
# include "clock.h"
# include "system/sched.h"
/*
	pulse all clocks
*/

void(*ffly_pulse)(ff_u64_t) = NULL;
static struct timespec start;

struct entry {
	struct timespec start;
	void(*func)(ff_u32_t);
	ff_u64_t rate; // every X ns
	ff_u64_t cur;
};

#define FUS 9
#define FUF 0.000001

static struct entry entries[] = {
	{.func=ffly_sched_clock_tick, .rate=SCHED_TIME_PERIOD/FUF, .cur=0},
	{.func=ffly_clock_tick, .rate=TIME_PERIOD/FUF, .cur=0}
};

#define N 2

ff_u64_t static buf = 0;
void static
pulse(ff_u64_t __nsec) {
	/*
		phy/clock /etc
	*/
	ff_u64_t np = (__nsec+buf)>>FUS;
	buf-=(buf>>FUS)*(1<<FUS);
	
	if (buf>(1<<FUS)) {
		ffly_printf("error.\n");
	}

	ff_u64_t ob;
	struct entry *ent = entries;
	struct entry *end = entries+N;
	while(ent != end) {
		if (ent->cur+np >= ent->rate) {
			ob = (ent->cur+np)-ent->rate;
			ent->func(1+(ff_u64_t)(ob/ent->rate));
			ent->cur = 0;
		}
		ent->cur+=np;
		ent++;
	}
	buf+=__nsec-(np*(1<<FUS));
}

void ffly_pengage(void) {
	ffly_pulse = pulse;	
}

void ffly_pdisengage(void) {
	ffly_pulse = NULL;
}
