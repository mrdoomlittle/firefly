# ifndef __ffly__io__h
# define __ffly__io__h
# include <mdlint.h>
# include <stdio.h>
# include "../types/err_t.h"
# define ffly_errmsg(...) ffly_printf(ffly_err, __VA_ARGS__)
# ifdef __cplusplus
extern "C" {
# endif
extern FILE *ffly_out;
extern FILE *ffly_log;
extern FILE *ffly_err;
ffly_err_t ffly_io_init();
void ffly_io_closeup();
void ffly_printf(FILE*, char const*, ...);
# ifdef __cplusplus
}
# endif

# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace system {
namespace io {
static void(*printf)(FILE*, char const*, ...) = &ffly_printf;
}
}
}
}
# endif
# endif /*__ffly__io__h*/
