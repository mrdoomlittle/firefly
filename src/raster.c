# include "raster.h"
# include "maths/max.h"
# include "maths/min.h"
# include "maths/barycentric.h"
# include "dep/bcopy.h"
# include "dep/mem_set.h"
# include "maths/rotate.h"
# include "maths/abs.h"
# include "maths/round.h"
# include "system/io.h"
# include "tri.h"
# include "tex.h"
void ffly_rraster(struct rr_struc *__s) {
	switch(__s->op) {
		case 0x00:
			ffly_printf("raster okay.\n");
		break;
		default:
			ffly_printf("unknown raster operation, got: %u\n", __s->op);
	}
}

//# define use_barycentric
void ffly_rasterize(ffly_modelp __model,
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

		struct ffly_tri2 tri = {
			.v0 = {v0.x, v0.y},
			.v1 = {v1.x, v1.y},
			.v2 = {v2.x, v2.y}
		};

		struct ffly_tex tx;
		tx.inn[0] = c[0];
		tx.inn[1] = c[1];
		tx.inn[2] = c[2];
		tx.inn[3] = c[3];

		ff_u16_t tex;

		ffly_printf("#}}}}}}}}} %u, %u\n", __x, __y);
		tex = ffly_g_tex(&tx);
		ffly_g_tri2(&tri, tex, __x, __y);
		i++;
	}
}
