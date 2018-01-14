# ifndef __ffly__system__string__h
# define __ffly__system__string__h
# include <mdlint.h>
# ifdef __cplusplus
extern "C" {
# endif
mdl_u64_t ffly_stno(char*);
mdl_uint_t ffly_nots(mdl_u64_t, char*);
double ffly_stfloat(char*);
mdl_uint_t ffly_floatts(double, char*);
char ffly_tolow(char);
mdl_u64_t ffly_htint(char*);
mdl_uint_t ffly_noths(mdl_u64_t, char*);
mdl_i8_t ffly_islen(char*, mdl_uint_t);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__system__string__h*/
