# ifndef __ffly__system__h
# define __ffly__system__h
# include "types.h"
# include "linux/types.h"
# ifdef __cplusplus
extern "C" {
# endif
void ff_set_ppid(__linux_pid_t);
void ff_setpid();
__linux_pid_t ff_get_ppid();
__linux_pid_t ff_getpid();

ff_err_t ffly_system_init();
ff_err_t ffly_system_de_init();
# ifdef __cplusplus
}
# endif
# endif /*__ffly__system__h*/
