# ifndef __ffly__linux__signal__h
# define __ffly__linux__signal__h
# include <mdlint.h>
# include "types.h"
mdl_s32_t ffly_kill(__linux_pid_t, mdl_s32_t);
# ifndef SIGKILL
# define SIGKILL 9
# endif
# endif /*__ffly__linux__signal__h*/
