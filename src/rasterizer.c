# include "rasterizer.h"
# include "maths/max.h"
# include "maths/min.h"
# include "maths/barycentric.h"
# include "dep/bcopy.h"
# include "dep/mem_set.h"
# include "maths/rotate.h"
# include "maths/abs.h"
# include "maths/round.h"
# include "system/io.h"

void ffly_rasterize(ffly_polygonp *__polylist, ff_byte_t *__dst,
	ff_uint_t __x, ff_uint_t __y,
	ff_uint_t __width, ff_uint_t __height,
	ff_uint_t __xmax, ff_uint_t __ymax)
{
	ffly_polygonp *cur, poly;
	ffly_vertex v0, v1, v2;

	cur = __polylist;
	while((poly = *cur) != NULL) {
	// so we can see
		poly->r = 0;
		poly->g = 0;
		poly->b = 0xff;
		poly->a = 0xff;

		v0 = *poly->vertices;
		v1 = poly->vertices[1];
		v2 = poly->vertices[2];

		ff_int_t xmin = ffly_min3(v0.x, v1.x, v2.x);
		ff_int_t ymin = ffly_min3(v0.y, v1.y, v2.y);
	
		ff_int_t xmax = ffly_max3(v0.x, v1.x, v2.x);
		ff_int_t ymax = ffly_max3(v0.y, v1.y, v2.y);

		float a, b, g;
		ff_int_t x, y = ymin;
		while(y != ymax && ffly_abs(ymin-y)+__y < __ymax) {
			x = xmin;
			while(x != xmax && ffly_abs(xmin-x)+__x < __xmax) {
				ffly_barycentric(x, y, 0, &v0, &v1, &v2, &a, &b, &g);
				if ((a>=0.0&&b>=0.0&&g>=0.0) || (x == xmin || y == ymin || x == xmax || y == ymax)) {
					ff_uint_t x0 = ffly_abs(xmin-x);
					ff_uint_t y0 = ffly_abs(ymin-y);
					ff_byte_t *dst = __dst+(((__x+x0)+((__y+y0)*__height))*4);

					*dst = poly->r;
					dst[1] = poly->g;
					dst[2] = poly->b;
					dst[3] = poly->a;
				}
				x++;
			}
			y++;
		}
		cur++;
	}
}
