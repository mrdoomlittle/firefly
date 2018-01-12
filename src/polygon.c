# include "polygon.h"
# include "maths/max.h"
# include "maths/min.h"
# include "types/byte_t.h"
# include "maths/barycentric.h"
# include "data/bcopy.h"
# include "data/mem_set.h"
# include "maths/rotate.h"
# include "maths/abs.h"
# include "maths/round.h"
# include "system/io.h"
# include "types/id_t.h"
void static rotate_vertex(ffly_vertexp __vertex, mdl_uint_t __x, mdl_uint_t __y, float __angle) {
    double x = __vertex->x-(mdl_int_t)__x, y = __vertex->y-(mdl_int_t)__y;
    ffly_rotate(&x, &y, __angle);
    ffly_fprintf(ffly_log, "%lf, %lf from %d, %d - %u, %u\n", x, y, __vertex->x, __vertex->y, __x, __y);
    __vertex->x = ffly_round(x)+(mdl_int_t)__x;
    __vertex->y = ffly_round(y)+(mdl_int_t)__y;
    ffly_fprintf(ffly_log, "%d, %d\n",  __vertex->x, __vertex->y);
}

// cache this
void ffly_draw_polygon(ffly_polygonp __poly, ffly_byte_t *__frame, ffly_byte_t *__texture, mdl_uint_t __texw, mdl_uint_t __x, mdl_uint_t __y, mdl_uint_t __z, mdl_uint_t __width, mdl_uint_t __height, mdl_uint_t __xmax, mdl_uint_t __ymax, float __angle) {
    ffly_vertex v0 = *__poly->vertexs;
    ffly_vertex v1 = *(__poly->vertexs+1);
    ffly_vertex v2 = *(__poly->vertexs+2);

    rotate_vertex(&v0, 0, 0, __angle);
    rotate_vertex(&v1, 0, 0, __angle);
    rotate_vertex(&v2, 0, 0, __angle);

    mdl_int_t xmin = ffly_min3(v0.x, v1.x, v2.x);
    mdl_int_t ymin = ffly_min3(v0.y, v1.y, v2.y);

    mdl_int_t xmax = ffly_max3(v0.x, v1.x, v2.x);
    mdl_int_t ymax = ffly_max3(v0.y, v1.y, v2.y);

    float a, b, g;
    mdl_int_t x, y = ymin;
    while(y != ymax && ffly_abs(ymin-y)+__y < __ymax) {
        x = xmin;
        while(x != xmax && ffly_abs(xmin-x)+__x < __xmax) {
            ffly_barycentric(x, y, __z, &v0, &v1, &v2, &a, &b, &g);
            if (a>=0.0&&b>=0.0&&g>=0.0 || (x == xmin || y == ymin || x == xmax || y == ymax)) {
                mdl_uint_t x0 = ffly_abs(xmin-x);
                mdl_uint_t y0 = ffly_abs(ymin-y);
                ffly_byte_t *dst = __frame+(((__x+x0)+((__y+y0)*__height))*4);
                ffly_bcopy(dst, __texture+((x0+(y0*__texw))*4), 4);
            }
            x++;
        }
        y++;
    }
}
