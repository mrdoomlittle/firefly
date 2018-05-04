# include "polygon.h"
# include "maths/max.h"
# include "maths/min.h"
# include "maths/barycentric.h"
# include "dep/bcopy.h"
# include "dep/mem_set.h"
# include "maths/rotate.h"
# include "maths/abs.h"
# include "maths/round.h"
# include "system/io.h"
void static
rotate_vertex(ffly_vertexp __vertex, ff_uint_t __x, ff_uint_t __y, float __angle) {
	double x = __vertex->x-(ff_int_t)__x, y = __vertex->y-(ff_int_t)__y;
	ffly_rotate(&x, &y, __angle);
  //  ffly_fprintf(ffly_log, "lf, lf from %d, %d - %u, %u\n", x, y, __vertex->x, __vertex->y, __x, __y);
	__vertex->x = ffly_round(x)+(ff_int_t)__x;
	__vertex->y = ffly_round(y)+(ff_int_t)__y;
	//ffly_fprintf(ffly_log, "%d, %d\n",  __vertex->x, __vertex->y);
}

void
ffly_draw_polygon(ffly_polygonp __poly, ff_byte_t *__frame, ff_byte_t *__texture, ff_uint_t __texw,
	ff_uint_t __x, ff_uint_t __y, ff_uint_t __z, ff_uint_t __width, ff_uint_t __height,
	ff_uint_t __xmax, ff_uint_t __ymax, float __angle)
{
	ffly_vertex v0 = *__poly->vertices;
	ffly_vertex v1 = *(__poly->vertices+1);
	ffly_vertex v2 = *(__poly->vertices+2);

	rotate_vertex(&v0, 0, 0, __angle);
	rotate_vertex(&v1, 0, 0, __angle);
	rotate_vertex(&v2, 0, 0, __angle);

	ff_int_t xmin = ffly_min3(v0.x, v1.x, v2.x);
	ff_int_t ymin = ffly_min3(v0.y, v1.y, v2.y);

	ff_int_t xmax = ffly_max3(v0.x, v1.x, v2.x);
	ff_int_t ymax = ffly_max3(v0.y, v1.y, v2.y);

	float a, b, g;
	ff_int_t x, y = ymin;
	while(y != ymax && ffly_abs(ymin-y)+__y < __ymax) {
		x = xmin;
		while(x != xmax && ffly_abs(xmin-x)+__x < __xmax) {
			ffly_barycentric(x, y, __z, &v0, &v1, &v2, &a, &b, &g);
			if ((a>=0.0&&b>=0.0&&g>=0.0) || (x == xmin || y == ymin || x == xmax || y == ymax)) {
				ff_uint_t x0 = ffly_abs(xmin-x);
				ff_uint_t y0 = ffly_abs(ymin-y);
				ff_byte_t *dst = __frame+(((__x+x0)+((__y+y0)*__height))*4);
				ffly_bcopy(dst, __texture+((x0+(y0*__texw))*4), 4);
			}
			x++;
		}
		y++;
	}
}
