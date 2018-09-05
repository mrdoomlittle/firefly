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
# include "raster.h"
# include "model.h"
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
ffly_draw_polygon(ffly_polygonp __poly, ff_byte_t *__dst,
	ff_uint_t __x, ff_uint_t __y,
	ff_uint_t __width, ff_uint_t __height,
	ff_uint_t __xmax, ff_uint_t __ymax)
{
	ff_u32_t colour = 0;
	__poly->c = 0;
	ffly_model model = {
		__poly,
		1,
		&colour,
		0
	};

	ffly_rasterize(&model, __dst, __x, __y, __width, __height, __xmax, __ymax);
}
