# ifndef __ffly__system__string__h
# define __ffly__system__string__h
# include <mdlint.h>
# ifdef __cplusplus
extern "C" {
# endif
mdl_u64_t ffly_stno(char*);
double ffly_stfloat(char*);
char ffly_tolow(char);
mdl_u64_t ffly_htint(char*);
mdl_i8_t ffly_islen(char*, mdl_uint_t);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__system__string__h*/
