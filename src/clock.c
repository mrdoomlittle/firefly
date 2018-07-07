# include "clock.h"
// temp
ff_u64_t clock = 0;
void ffly_clock_tick(ff_u32_t __delta) {
	clock+=__delta;
}

ff_u64_t ffly_clock_get(void) {
	return clock;
}
