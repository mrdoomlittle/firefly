# ifndef __ffly__printf__h
# define __ffly__printf__h
# include <stdarg.h>
# include "../ffint.h"
# include "../types.h"
# ifdef __cplusplus
extern "C" {
# endif
ff_err_t ffly_printf(char const*, ...);
ff_err_t ffly_fprintf(FF_FILE*, char const*, ...);
ff_err_t ffly_vfprintf(FF_FILE*, char const*, va_list);
ff_err_t ffly_vsfprintf(FF_FILE*, ff_size_t, char const*, va_list);
ff_uint_t ffly_sprintf(char*, char const*, ...);
ff_uint_t ffly_vsprintf(char*, char const*, va_list);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__printf__h*/
