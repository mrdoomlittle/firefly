# include "../types.h"
# include "raster.h"
# include "../maths.h"
# include "types.h"
# include "raise.h"
# include "context.h"
# include "../system/io.h"
// need to be worked on but will do for now
/*
	only can rasterize tri
	or any other simple shapes if repurposed
*/
ff_i8_t
static dim(ff_int_t __x, ff_int_t __y, struct sr_vertex2 *__a, struct sr_vertex2 *__b, struct sr_vertex2 *__c) {
	struct sr_vertex2 v0 = {
		.x=__b->x-__a->x,
		.y=__b->y-__a->y
	};

	struct sr_vertex2 v1 = {
		.x=__c->x-__a->x,
		.y=__c->y-__a->y
	};

	struct sr_vertex2 v2 = {
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

//ff_i8_t static
//barycentric(ff_int_t __x, ff_int_t __y, struct sr_vertex2 *__a, struct sr_vertex2 *__b, struct sr_vertex2 *__c) {

//}

void sr_raster_tri2(void) {
	struct sr_tri2 *tri;

	tri = (struct sr_tri2*)(sr_raise_stack+(*(ff_u16_t*)sr_raise_p));

	struct sr_tex *tex;

	/*
		texture will be loaded diffrently but for now
	*/
	tex = (struct sr_tex*)(sr_raise_stack+(*(ff_u16_t*)(sr_raise_p+sizeof(ff_u16_t))));

	struct sr_vertex2 v0, v1, v2;

	v0 = tri->v0;
	v1 = tri->v1;
	v2 = tri->v2;

	ffly_printf("v0: %d, %d\nv1: %d, %d\nv2: %d, %d\n", v0.x, v0.y, v1.x, v1.y, v2.x, v2.y);
	ffly_printf("tex: %u, %u, %u, %u\n", tex->inn[0], tex->inn[1], tex->inn[2], tex->inn[3]);

	ff_int_t xmin = ffly_min3(v0.x, v1.x, v2.x);
	ff_int_t ymin = ffly_min3(v0.y, v1.y, v2.y);

	ff_int_t xmax = ffly_max3(v0.x, v1.x, v2.x);
	ff_int_t ymax = ffly_max3(v0.y, v1.y, v2.y);
	ffly_printf("xmin: %d, ymin: %d, xmax: %d, ymax: %d\n", xmin, ymin, xmax, ymax);

	struct sr_framebuff *fb;

	fb = sr_ctx->fb;

#ifdef use_barycentric
	float a, b, g;
#endif

	struct sr_tile *t, **tp;
	ff_uint_t tx, ty;
	ff_uint_t txo, tyo;
	ff_int_t x, y = ymin;
	while(y < ymax) {
		x = xmin;
		while(x < xmax) {
#ifdef use_barycentric
//			barycentric(x, y, 0, &v0, &v1, &v2, &a, &b, &g);
#else
			if (!dim(x, y, &v0, &v1, &v2)) {
#endif
#ifdef use_barycentric
			if (a>=0 && b>=0 && g>=0) {
#endif
				ff_uint_t x0 = abs(xmin-x);
				ff_uint_t y0 = abs(ymin-y);
				tx = x0>>TILESZ;
				ty = y0>>TILESZ;
				if (!(t = *(tp = tile_at(tx, ty, fb)))) {
					t = (*tp = sr_tile_new(TILESZ));
					sr_tile_map(t);
				}	
				txo = x0-(tx*(1<<TILESZ));
				tyo = y0-(ty*(1<<TILESZ));

				ff_byte_t *dst;
				dst = tilepx(t, txo, tyo);
				*(ff_u32_t*)dst = *(ff_u32_t*)tex->inn;
			}
			
			x++;
		}
		y++;
	}	
}
