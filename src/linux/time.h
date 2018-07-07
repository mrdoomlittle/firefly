# ifndef __ffly__linux__time__h
# define __ffly__linux__time__h
# include "../ffint.h"
# include "types.h"
# include "signal.h"
struct timespec {
	__linux_time_t tv_sec;
	ff_u64_t tv_nsec;
};

struct timeval {
	__linux_time_t tv_sec;
	__linux_suseconds_t tv_usec;
};

struct itimerspec {
	struct timespec it_interval;
	struct timespec it_value;
};

# define CLOCK_REALTIME 0
# define CLOCK_MONOTONIC 1

ff_s32_t clock_gettime(__linux_clockid_t, struct timespec*);
ff_s32_t timer_create(__linux_clockid_t, struct sigevent*, __linux_timer_t*);
ff_s32_t timer_delete(__linux_timer_t);
ff_s32_t timer_settime(__linux_timer_t, ff_u32_t, struct itimerspec*, struct itimerspec*);
# endif /*__ffly__linux__time__h*/
