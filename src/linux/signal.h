# ifndef __ffly__linux__signal__h
# define __ffly__linux__signal__h
# include <mdlint.h>
# include "types.h"
mdl_s32_t kill(__linux_pid_t, mdl_s32_t);
# define SIGKILL 9
# define SIGCHLD 18
# endif /*__ffly__linux__signal__h*/
