# ifndef __io__h
# define __io__h
# include <mdlint.h>
# include <stdio.h>
# ifdef __cplusplus
extern "C" {
# endif
void ffly_printf(FILE*, char const*, ...);
# define ffly_errmsg(__MSG, __VA_ARGS) ffly_printf(stderr, __MSG, __VA_ARGS);
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

# endif /*__io__h*/
