# ifndef __ffly__linux__uio__h
# define __ffly__linux__uio__h
# include "../ffint.h"
# include "types.h"
struct iovec {
	void *base;
	__linux_size_t size;
};

ff_s32_t writev(ff_u32_t, struct iovec*, ff_u32_t);
ff_s32_t readv(ff_u32_t, struct iovec*, ff_u32_t);
# endif /*__ffly__linux__uio__h*/
