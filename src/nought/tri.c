# include "../types.h"
# include "raster.h"
# include "../maths.h"
# include "types.h"
# include "raise.h"
# include "context.h"
# include "../system/io.h"
# include "pixel.h"
# include "tex.h"
// need to be worked on but will do for now
/*
	only can rasterize tri
	or any other simple shapes if repurposed
*/
ff_i8_t
static dim(ff_int_t __x, ff_int_t __y, struct nt_vertex2 *__a, struct nt_vertex2 *__b, struct nt_vertex2 *__c) {
	struct nt_vertex2 v0 = {
		.x=__b->x-__a->x,
		.y=__b->y-__a->y
	};

	struct nt_vertex2 v1 = {
		.x=__c->x-__a->x,
		.y=__c->y-__a->y
	};

	struct nt_vertex2 v2 = {
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
//barycentric(ff_int_t __x, ff_int_t __y, struct nt_vertex2 *__a, struct nt_vertex2 *__b, struct nt_vertex2 *__c) {

//}

void nt_raster_tri2(void) {
	struct nt_tri2 *tri;

	tri = (struct nt_tri2*)(nt_raise_stack+(*(ff_u16_t*)nt_raise_p));

	struct nt_tex *tex;

	/*
		texture will be loaded diffrently but for now
	*/
	tex = *(struct nt_tex**)(nt_raise_stack+(*(ff_u16_t*)(nt_raise_p+2)));
	ff_u32_t _x, _y;

	_x = *(ff_u32_t*)(nt_raise_p+4);
	_y = *(ff_u32_t*)(nt_raise_p+8);

	ffly_printf("--------> %ux%u\n", _x, _y);
	struct nt_vertex2 v0, v1, v2;

	v0 = tri->v0;
	v1 = tri->v1;
	v2 = tri->v2;

	ffly_printf("v0: %d, %d\nv1: %d, %d\nv2: %d, %d\n", v0.x, v0.y, v1.x, v1.y, v2.x, v2.y);

	ff_int_t xmin = ffly_min3(v0.x, v1.x, v2.x);
	ff_int_t ymin = ffly_min3(v0.y, v1.y, v2.y);

	ff_int_t xmax = ffly_max3(v0.x, v1.x, v2.x);
	ff_int_t ymax = ffly_max3(v0.y, v1.y, v2.y);
	ffly_printf("xmin: %d, ymin: %d, xmax: %d, ymax: %d\n", xmin, ymin, xmax, ymax);

	struct nt_framebuff *fb;

	fb = nt_ctx->fb;

#ifdef use_barycentric
	float a, b, g;
#endif

	struct nt_tile *t, **tp;
	ff_uint_t tx, ty;
	ff_uint_t txo, tyo;
	ff_int_t x, y = ymin;
	ff_u8_t *c;
	c = (ff_u8_t*)tex->b->data;

	ffly_printf("tri2 texture: %u\n", tex->b->size);
	while(y<ymax) {
		x = xmin;
		while(x<xmax) {
#ifdef use_barycentric
//			barycentric(x, y, 0, &v0, &v1, &v2, &a, &b, &g);
#else
			if (!dim(x, y, &v0, &v1, &v2)) {
#endif
#ifdef use_barycentric
			if (a>=0 && b>=0 && g>=0) {
#endif
				ff_uint_t x0 = _x+abs(xmin-x);
				ff_uint_t y0 = _y+abs(ymin-y);
				tx = x0>>TILESZ;
				ty = y0>>TILESZ;
				if (!(t = *(tp = tile_at(tx, ty, fb)))) {
					t = (*tp = nt_tile_new(TILESZ));
					nt_tile_map(t);
				}	
				txo = x0-(tx*(1<<TILESZ));
				tyo = y0-(ty*(1<<TILESZ));

				ff_byte_t *dst;
				dst = tilepx(t, txo, tyo);
				nt_setpix(c, dst);
	//			c+=4;
			}
			
			x++;
		}
		y++;
	}	
}
