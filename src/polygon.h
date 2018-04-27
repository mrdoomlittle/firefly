# ifndef __ffly__polygon__h
# define __ffly__polygon__h
# include "vertex.h"
# include "ffint.h"
# include "types.h"
# ifndef ffly_vertex3
#   define ffly_vertex3(__poly, __x, __y, __z) \
        *((__poly)->vertexs+((__poly)->vertex_c++)) = (ffly_vertex){.x=__x, .y=__y, .z=__z}
# endif
# ifndef ffly_polygon_begin

# endif
# ifndef ffly_polygon_end
# endif

typedef struct {
    ffly_vertex vertexs[20];
    ff_uint_t vertex_c;
} ffly_polygon;

typedef ffly_polygon* ffly_polygonp;
void ffly_draw_polygon(ffly_polygonp, ff_byte_t*, ff_byte_t*, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t, float);
# endif /*__ffly__polygon__h*/
