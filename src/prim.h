# ifndef __ffly__prim__h
# define __ffly__prim__h
# include "ffint.h"
struct ffly_vertex2 {
	ff_int_t x, y;
};

struct ffly_tri2 {
	struct ffly_vertex2 v0, v1, v2;
};

struct ffly_tex {
	ff_u8_t inn[4];
};

# endif /*__ffly__prim__h*/
