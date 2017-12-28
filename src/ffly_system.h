# ifndef __ffly__system__h
# define __ffly__system__h
# include <sys/types.h>
pid_t extern ffly_ppid;
# ifdef __cplusplus
extern "C" {
# endif
void ffly_set_ppid(pid_t);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__system__h*/
