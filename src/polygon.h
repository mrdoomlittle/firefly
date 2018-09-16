# ifndef __ffly__polygon__h
# define __ffly__polygon__h
# include "vertex.h"
# include "ffint.h"
# include "types.h"
#ifndef ffly_vertex3
# define ffly_vertex3(__poly, __x, __y, __z) \
	*((__poly)->vertices+((__poly)->vertex_c++)) = (ffly_vertex){.x=__x, .y=__y, .z=__z}
#endif
typedef struct {
	ffly_vertex vertices[4];
    ff_uint_t vertex_c;
	ff_u32_t c; // points to colour within texture c = {offset from base}>>2
} ffly_polygon;

typedef ffly_polygon* ffly_polygonp;
void ffly_draw_polygon(ffly_polygonp, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t);

# endif /*__ffly__polygon__h*/
