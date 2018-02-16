# ifndef __ffly__stdio__h
# define __ffly__stdio__h
# include "system/io.h"
# define stdout ffly_out
# define stderr ffly_out
# define printf(...) \
	ffly_printf(__VA_ARGS__)
# define fprintf(...) \
	ffly_fprintf(__VA_ARGS__)
# endif /*__ffly__stdio__h*/
