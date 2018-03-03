# ifndef __ffly__string__h
# define __ffly__string__h
# include "dep/str_len.h"
# include "dep/str_cmp.h"
# include "system/string.h"
# include "dep/str_dup.h"
# include "dep/mem_cpy.h"
# define memcpy(__dst, __src, __n) \
	ffly_mem_cpy(__dst, __src, __n)
# define strlen(__s) \
	ffly_str_len(__s)
# define memcmp(__p0, __p1, __n) \
	ffly_mem_cmp(__p0, __p1, __n)
# define strcmp(__s0, __s1) \
	ffly_str_cmp(__s0, __s1)
# endif /*__ffly__string__h*/
