# ifndef __ffly__linux__fcntl__h
# define __ffly__linux__fcntl__h
# include <mdlint.h>

# define O_RDONLY 00000000
# define O_WRONLY 00000001
# define O_RDWR 00000002

# define O_CREAT 0000010
# define O_TRUNC 00001000

# define F_GETFD 1

mdl_s32_t fcntl(mdl_u32_t, mdl_s32_t, mdl_u64_t);
mdl_s32_t close(mdl_u32_t);
mdl_s32_t creat(char const*, mdl_s32_t);
# endif /*__ffly__linux__fcntl__h*/
