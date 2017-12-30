# ifndef __ffly__system__h
# define __ffly__system__h
# include "types/err_t.h"
# include <sys/types.h>
pid_t extern ffly_ppid;
# ifdef __cplusplus
extern "C" {
# endif
void ffly_set_ppid(pid_t);
ffly_err_t ffly_system_init();
ffly_err_t ffly_system_de_init();
# ifdef __cplusplus
}
# endif
# endif /*__ffly__system__h*/
