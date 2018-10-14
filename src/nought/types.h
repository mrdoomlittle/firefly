# ifndef __ffly__nt__types__h
# define __ffly__nt__types__h
# include "../ffint.h"

struct nt_tex {
	ff_u8_t inn[4];
};

struct nt_vertex2 {
	ff_int_t x, y;
};

struct nt_tri2 {
	struct nt_vertex2 v0, v1, v2;
};

# endif /*__ffly__nt__types__h*/
