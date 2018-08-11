# ifndef __ffly__hs__vec__h
# define __ffly__hs__vec__h
# include "../ffint.h"
typedef struct ff_hs_vec {
	void *p;
	ff_u16_t blksize;
	ff_uint_t off, page_c;
} *ff_hs_vecp;

# define ff_hs_vec_front(__vec) \
	((__vec)->p)
# define ff_hs_vec_rear(__vec) \
	(((ff_u8_t*)(__vec)->p)+((__vec)->off*(__vec)->blksize))

void ff_hs_vec_init(ff_hs_vecp, ff_u16_t);
void* ff_hs_vec_push(ff_hs_vecp);
# endif /*__ffly__hs__vec__h*/
