# ifndef __ffly__io__h
# define __ffly__io__h
# include <mdlint.h>
# include <stdio.h>
# include "../types/err_t.h"
# define ffly_errmsg(...) ffly_fprintf(ffly_err, __VA_ARGS__)
# define ffly_logmsg(...) ffly_fprintf(ffly_log, __VA_ARGS__)
# ifdef __cplusplus
extern "C" {
# endif
extern FILE *ffly_out;
extern FILE *ffly_log;
extern FILE *ffly_err;
ffly_err_t ffly_io_init();
void ffly_io_closeup();
void ffly_fprintf(FILE*, char const*, ...);
void ffly_printf(char const*, ...);
# ifdef __cplusplus
}
# endif

# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace system {
namespace io {
static void(*fprintf)(FILE*, char const*, ...) = &ffly_fprintf;
static void(*printf)(char const*, ...) = &ffly_printf;
}
}
}
}
# endif
# endif /*__ffly__io__h*/
