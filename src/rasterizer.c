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

// need to be worked on but will do for now
ff_i8_t
static dim(ff_int_t __x, ff_int_t __y, ffly_vertexp __a, ffly_vertexp __b, ffly_vertexp __c) {
	ffly_vertex v0 = {
		.x=__b->x-__a->x,
		.y=__b->y-__a->y
	};

	ffly_vertex v1 = {
		.x=__c->x-__a->x,
		.y=__c->y-__a->y
	};

	ffly_vertex v2 = {
		.x=__x-__a->x,
		.y=__y-__a->y
	};

	float x, y;
	x = v2.x;
	y = v2.y;

	float f0, f1, f2, f3, f4, f5;

	f0 = x*(__c->y-__a->y);
	f1 = (__c->x-__a->x)*y;

	f2 = x*(__a->y-__b->y);
	f3 = (__a->x-__b->x)*y;

	f4 = (v1.x-x)*(__c->y-__b->y);
	f5 = (__c->x-__b->x)*(v1.y-y);

	if ((f0-f1)*(f2-f3) >= 0 && (f2-f3)*(f4-f5) >= 0 && (f0-f1)*(f4-f5) >= 0)
		return 0;
	return -1;
}

//# define use_barycentric
void ffly_rasterize(ffly_modelp __model, ff_byte_t *__dst,
	ff_uint_t __x, ff_uint_t __y,
	ff_uint_t __width, ff_uint_t __height,
	ff_uint_t __xmax, ff_uint_t __ymax)
{
	if (!__model->texture) {
		ffly_printf("model has no texture.\n");
		return;
	}

	ffly_polygonp poly;
	ffly_vertex v0, v1, v2;

	ff_uint_t i;
	ff_uint_t poly_c;
	poly_c = __model->poly_c;

	i = 0;
	while(i != poly_c) {
		poly = __model->poly+i;
		ff_byte_t *c;

		// *4
		c = __model->texture+(poly->c<<2);

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
# ifdef use_barycentric
				float a, b, g;
				ffly_barycentric(x, y, 0, &v0, &v1, &v2, &a, &b, &g);
# else
				if (!dim(x, y, &v0, &v1, &v2)) {
# endif
# ifdef use_barycentric
				if (a>=0 && b>=0 && c>=0) {
# endif
					ff_uint_t x0 = ffly_abs(xmin-x);
					ff_uint_t y0 = ffly_abs(ymin-y);
					ff_byte_t *dst = __dst+(((__x+x0)+((__y+y0)*__height))*4);

					*(ff_u32_t*)dst = *(ff_u32_t*)c;
				}
				x++;
			}
			y++;
		}
		i++;
	}
}
