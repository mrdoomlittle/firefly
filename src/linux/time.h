# ifndef __ffly__linux__time__h
# define __ffly__linux__time__h
# include "../ffint.h"
# include "types.h"
struct timespec {
	__linux_time_t tv_sec;
	ff_u64_t tv_nsec;
};

struct timeval {
	__linux_time_t tv_sec;
	__linux_suseconds_t tv_usec;
};

# define CLOCK_REALTIME 0
# define CLOCK_MONOTONIC 1

ff_s32_t clock_gettime(__linux_clockid_t, struct timespec*);
# endif /*__ffly__linux__time__h*/
