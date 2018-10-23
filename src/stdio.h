# ifndef __ffly__stdio__h
# define __ffly__stdio__h
# include "system/io.h"
#ifndef stdout
# define stdout ffly_out
#endif
#ifndef stderr
# define stderr ffly_out
#endif
#ifndef printf
# define printf(...) \
	ffly_printf(__VA_ARGS__)
#endif
#ifndef fprintf
# define fprintf(...) \
	ffly_fprintf(__VA_ARGS__)
#endif
# endif /*__ffly__stdio__h*/
