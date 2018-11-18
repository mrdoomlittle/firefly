# ifndef __ffly__system__string__h
# define __ffly__system__string__h
# include "../ffint.h"
# define CUT_SHIFT 3
# define CUT_SIZE (1<<CUT_SHIFT)
# define islrc(__c) \
	((__c)>='a'&&(__c)<='z')
# define ishrc(__c) \
	((__c)>='A'&&(__c)<='Z')
# ifdef __cplusplus
extern "C" {
# endif

// string of x size to number
ff_u64_t ffly_sntno(char*, ff_uint_t);
ff_u64_t ffly_stno(char*);
ff_uint_t ffly_nots(ff_u64_t, char*);
ff_uint_t _ffly_nots(ff_u64_t, ff_u16_t*, ff_u8_t*, void(*)(ff_u8_t*, ff_u16_t, ff_u16_t, ff_u8_t));
double ffly_stfloat(char*);
ff_uint_t ffly_floatts(double, char*);
ff_uint_t _ffly_floatts(double, ff_u16_t*, ff_u8_t*, void(*)(ff_u8_t*, ff_u16_t, ff_u16_t, ff_u8_t));
char ffly_tolow(char);
ff_u64_t ffly_htint(char*);
ff_uint_t ffly_noths(ff_u64_t, char*);
ff_uint_t _ffly_noths(ff_u64_t, ff_u16_t*, ff_u8_t*, void(*)(ff_u8_t*, ff_u16_t, ff_u16_t, ff_u8_t));
ff_i8_t ffly_islen(char*, ff_uint_t);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__system__string__h*/
