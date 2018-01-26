# ifndef __ffly__printf__h
# define __ffly__printf__h
# include <stdarg.h>
# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_printf(char const*, ...);
ffly_err_t ffly_fprintf(FF_FILE*, char const*, ...);
ffly_err_t ffly_vfprintf(FF_FILE*, char const*, va_list);
ffly_err_t ffly_vsfprintf(FF_FILE*, ffly_size_t, char const*, va_list);
mdl_uint_t ffly_sprintf(char*, char const*, ...);
mdl_uint_t ffly_vsprintf(char*, char const*, va_list);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__printf__h*/
