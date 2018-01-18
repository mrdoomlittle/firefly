# ifndef __ffly__io__h
# define __ffly__io__h
# include <mdlint.h>
# include <stdio.h>
# include "../types/err_t.h"
# include "../types/fd_t.h"
# include "../types/size_t.h"
# include "file.h"
# include "printf.h"
# define ffly_errmsg(...) ffly_fprintf(ffly_err, __VA_ARGS__)
# define ffly_logmsg(...) ffly_fprintf(ffly_log, __VA_ARGS__)
# ifdef __cplusplus
extern "C" {
# endif
extern FF_FILE *ffly_out;
extern FF_FILE *ffly_log;
extern  FF_FILE *ffly_err;
ffly_err_t ffly_io_init();
void ffly_io_closeup();
mdl_int_t ffly_open(char const*, mdl_int_t, mdl_int_t);
mdl_int_t ffly_close(mdl_int_t);
/*
ffly_size_t ffly_write(ffly_fd_t, void*, ffly_size_t, ffly_err_t*);
ffly_size_t ffly_read(ffly_fd_t, void*, ffly_size_t, ffly_err_t*);
*/
# ifdef __cplusplus
}
# endif

# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace system {
namespace io {
static types::err_t(*fprintf)(FF_FILE*, char const*, ...) = &ffly_fprintf;
static types::err_t(*printf)(char const*, ...) = &ffly_printf;
}
}
}
}
# endif
# endif /*__ffly__io__h*/
