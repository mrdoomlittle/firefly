# ifndef __ffly__dus__mm__h
# define __ffly__dus__mm__h
# include "../ffint.h"
void* ff_dus_mmap(ff_uint_t);
void ff_dus_munmap(void*);
void ff_dus_mm_cleanup(void);
# endif /*__ffly__dus__mm__h*/
