# ifndef __ffly__clock__h
# define __ffly__clock__h
# include "ffint.h"
# define TIME_PERIOD 0.001
ff_u64_t extern clock;

void ffly_clock_tick(void);
ff_u64_t ffly_clock_get(void);
# endif /*__ffly__clock__h*/
