# ifndef __ffly__printf__h
# define __ffly__printf__h
# include <stdarg.h>
# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_printf(char const*, ...);
ffly_err_t ffly_fprintf(FF_FILE*, char const*, ...);
ffly_err_t ffly_vfprintf(FF_FILE*, char const*, va_list __args);
ffly_err_t ffly_vsfprintf(FF_FILE*, ffly_size_t, char const*, va_list __args);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__printf__h*/
