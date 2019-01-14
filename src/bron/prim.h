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
	ff_i32_t x0, y0;
	ff_i32_t x1, y1;
	ff_i32_t x2, y2;
} __attribute__((packed));

struct bron_tri3 {
	ff_i32_t x0, y0, z0;
	ff_i32_t x1, y1, z1;
	ff_i32_t x2, y2, z2;
} __attribute__((packed));

# endif /*__ffly__prim__h*/
