# ifndef __ffly__nt__types__h
# define __ffly__nt__types__h
# include "../ffint.h"

struct nt_vertex2 {
	ff_int_t x, y;
} __attribute__((packed));

struct nt_tri2 {
	struct nt_vertex2 v0, v1, v2;
} __attribute__((packed));

struct nt_tri3 {
	ff_s32_t x0, y0, z0;
	ff_s32_t x1, y1, z1;
	ff_s32_t x2, y2, z2;
} __attribute__((packed));

# endif /*__ffly__nt__types__h*/
