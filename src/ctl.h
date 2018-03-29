# ifndef __ffly__ctl__h
# define __ffly__ctl__h
# include <mdlint.h>
# include "memory/allocr.h"
// memory allocator control
# define ffly_malc ffly_arctl
# define ffly_ctl(__func, __req, __val) \
	(__func)(__req, __val)
# endif /*__ffly__ctl__h*/
