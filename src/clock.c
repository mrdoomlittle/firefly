# include "clock.h"
ff_u64_t clock = 0;
void ffly_clock_tick(void) {
	clock++;
}

ff_u64_t ffly_clock_get(void) {
	return clock;
}
