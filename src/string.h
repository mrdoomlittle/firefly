# ifndef __ffly__string__h
# define __ffly__string__h
# include "dep/str_len.h"
# include "dep/str_cmp.h"
# include "system/string.h"
# include "dep/str_dup.h"
# include "dep/mem_cpy.h"
# define memcpy(__dst, __src, __n) \
	ffly_mem_cpy(__dst, __src, __n)
# endif /*__ffly__string__h*/
