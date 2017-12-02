# ifndef __ffly__dir__h
# define __ffly__dir__h
# include <mdlint.h>
# include <sys/types.h>
# include "../types/err_t.h"
ffly_err_t ffly_mkdir(char*, mode_t);
ffly_err_t ffly_rmdir(char*);
# endif /*__ffly__dir__h*/
