# ifndef __ffly__br__h
# define __ffly__br__h
# include "ffint.h"
# include "net.h"
# include "brick.h"
typedef struct ffly_br {
	FF_SOCKET *sock;
	ff_u32_t *bricks;
	ff_uint_t brick_c;
	ff_u8_t bricksize;
} *ffly_brp;

void ffly_br_retrieve(ff_u8_t, ff_uint_t, void*);
void ffly_br_put(ffly_brp, ff_u32_t, ff_uint_t);
void ffly_br_free(ffly_brp);
void ffly_br_prep(ffly_brp, ff_u8_t, ff_uint_t);
void ffly_br_shutdown(ffly_brp);
void ffly_br_open(ffly_brp);
void ffly_br_close(ffly_brp);
void ffly_br_start(ffly_brp);
# endif /*__ffly__br__h*/
