# ifndef __ffly__polygon__h
# define __ffly__polygon__h
# include "types/vertex_t.h"
# include <mdlint.h>
# include "types/byte_t.h"
# ifndef ffly_vertex3
#   define ffly_vertex3(__polygon, __xa, __ya, __za) \
        *((__polygon)->vertexs+((__polygon)->vertex_c++)) = (ffly_vertex_t){.xa=__xa, .ya=__ya, .za=__za}
# endif
# ifndef ffly_polygon_begin

# endif
# ifndef ffly_polygon_end
# endif

struct ffly_polygon {
    ffly_vertex_t vertexs[20];
    mdl_uint_t vertex_c;
};

typedef struct ffly_polygon* ffly_polygonp;
# endif /*__ffly__polygon__h*/
