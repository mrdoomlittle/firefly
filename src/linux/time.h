# ifndef __ffly__linux__time__h
# define __ffly__linux__time__h
# include "types.h"
struct timespec {
	__linux_time_t tv_sec;
	mdl_u64_t tv_nsec;
};

# define CLOCK_REALTIME 0
# define CLOCK_MONOTONIC 1
# endif /*__ffly__linux__time__h*/
