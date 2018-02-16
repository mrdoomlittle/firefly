# ifndef __ffly__string__h
# define __ffly__string__h
# include "data/str_len.h"
# include "data/str_cmp.h"
# include "data/str_cmb.h"
# include "system/string.h"
# include "data/str_dupe.h"
# include "data/mem_cpy.h"
# define memcpy(__dst, __src, __n) \
	ffly_mem_cpy(__dst, __src, __n)
# endif /*__ffly__string__h*/
