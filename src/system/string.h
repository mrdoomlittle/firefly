# ifndef __ffly__system__string__h
# define __ffly__system__string__h
# include "../ffint.h"
# define islrc(__c) \
	((__c)>='a'&&(__c)<='z')
# define ishrc(__c) \
	((__c)>='A'&&(__c)<='Z')
# ifdef __cplusplus
extern "C" {
# endif
ff_u64_t ffly_stno(char*);
ff_uint_t ffly_nots(ff_u64_t, char*);
double ffly_stfloat(char*);
ff_uint_t ffly_floatts(double, char*);
char ffly_tolow(char);
ff_u64_t ffly_htint(char*);
ff_uint_t ffly_noths(ff_u64_t, char*);
ff_i8_t ffly_islen(char*, ff_uint_t);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__system__string__h*/
