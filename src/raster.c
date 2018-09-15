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

	
		i++;
	}
}
