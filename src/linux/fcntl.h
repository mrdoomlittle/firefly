# ifndef __ffly__linux__fcntl__h
# define __ffly__linux__fcntl__h
# include "../ffint.h"
#define O_RDONLY 00000000
#define O_WRONLY 00000001
#define O_RDWR 00000002

#define O_CREAT 00000100
#define O_TRUNC 00001000
#define O_APPEND 00002000

#define O_NONBLOCK 00004000

#define F_GETFD 1

ff_s32_t fcntl(ff_u32_t, ff_u32_t, ff_u64_t);
ff_s32_t close(ff_u32_t);
ff_s32_t creat(char const*, ff_u32_t);
# endif /*__ffly__linux__fcntl__h*/
