# include "polygon.h"
# include "maths/max.h"
# include "maths/min.h"
# include "types/byte_t.h"
# include "maths/barycentric.h"
# include "data/bcopy.h"
# include "data/mem_set.h"
void ffly_draw_polygon(ffly_polygonp __poly, ffly_byte_t *__frame, ffly_byte_t *__texture, mdl_uint_t __x, mdl_uint_t __y, mdl_uint_t __width, mdl_uint_t __height, mdl_uint_t __xmax, mdl_uint_t __ymax) {
    mdl_uint_t x, y;
    ffly_vertexp v0 = __poly->vertexs;
    ffly_vertexp v1 = __poly->vertexs+1;
    ffly_vertexp v2 = __poly->vertexs+2;

    mdl_uint_t xmin = ffly_min3(v0->x, v1->x, v2->x);
    mdl_uint_t ymin = ffly_min3(v0->y, v1->y, v2->y);

    mdl_uint_t xmax = ffly_max3(v0->x, v1->x, v2->x);
    mdl_uint_t ymax = ffly_max3(v0->y, v1->y, v2->y);

    float a, b, g;
    y = ymin;
    while(y != ymax && y+__y < __ymax) {
        x = xmin;
        while(x != xmax && x+__x < __xmax) {
            ffly_barycentric(x, y, v0, v1, v2, &a, &b, &g);
            if (a>=0.0&&b>=0.0&&g>=0.0) {
                ffly_mem_set(__frame+(((__x+x)+((__y+y)*__height))*4), 200, 4);
                //ffly_bcopy(__frame+(((__x+x)+((__y+y)*__height))*4), /*__texture+((x+(y*ymax))*4)*/'#', 4);
            }
            x++;
        }
        y++;
    }
}
