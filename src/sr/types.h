# ifndef __ffly__sr__types__h
# define __ffly__sr__types__h
# include "../ffint.h"

struct sr_tex {
	ff_u8_t inn[4];
};

struct sr_vertex2 {
	ff_int_t x, y;
};

struct sr_tri2 {
	struct sr_vertex2 v0, v1, v2;
};

# endif /*__ffly__sr__types__h*/
