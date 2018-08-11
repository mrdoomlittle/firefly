# ifndef __ffly__m__h
# define __ffly__m__h
# include "ffint.h"
struct ffly_meminfo {
# ifdef __ffly_debug
	ff_uint_t used;
	ff_uint_t ac, fc;
# endif
};

ff_u64_t ffly_mem_usage(void);
// display memory info
void ffly_dmi(struct ffly_meminfo*);
void ffly_meminfo(struct ffly_meminfo*);
# endif /*__ffly_m__h*/
