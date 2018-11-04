# ifndef __ffly__prim__h
# define __ffly__prim__h
# include "../ffint.h"

# define bron_vertex2 \
	ffly_bron_vertex2
# define bron_tri2 \
	ffly_bron_tri2
struct ffly_bron_vertex2 {
	ff_int_t x, y;
};

struct ffly_bron_tri2 {
	struct ffly_bron_vertex2 v0, v1, v2;
};

# endif /*__ffly__prim__h*/
