# include "clock.h"
double clock = 0.0;
void ffly_clock_tick(void) {
	clock+=TIME_PERIOD;
}
