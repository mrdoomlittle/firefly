# ifndef __ffly__pellet__h
# define __ffly__pellet__h
# include "ffint.h"
# define ffly_pellet_puti(__pel, __p, __sz) \
	ffly_pellet_put(__pel, __p, __sz); \
	ffly_pellet_incr(__pel, __sz)
# define ffly_pellet_getd(__pel, __p, __sz) \
	ffly_pellet_decr(__pel, __sz); \
	ffly_pellet_get(__pel, __p, __sz)

typedef struct ffly_pellet {
	ff_u8_t *p, *end;
	ff_uint_t off;
} *ffly_pelletp;

ffly_pelletp ffly_pellet_mk(ff_uint_t);
void ffly_pellet_put(ffly_pelletp, void const*, ff_uint_t);
void ffly_pellet_get(ffly_pelletp, void*, ff_uint_t);
void ffly_pellet_incr(ffly_pelletp, ff_uint_t);
void ffly_pellet_decr(ffly_pelletp, ff_uint_t);
void ffly_pellet_free(ffly_pelletp);
# endif /*__ffly__pellet__h*/
