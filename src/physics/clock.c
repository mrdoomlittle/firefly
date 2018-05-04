# include "clock.h"
ff_u32_t clock = 0;
void ffly_clock_tick(void) {
	clock++;
}
