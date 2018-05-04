# include "clock.h"
/*
	using real time would take up more time,
	so we will just simulate it
*/

/*
	each cycle = TIME_PERIOD
*/
ff_u32_t clock = 0;
void ffly_clock_tick(void) {
	clock++;
}
