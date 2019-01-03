# include "../types.h"
# include "raster.h"
# include "../maths.h"
# include "types.h"
# include "raise.h"
# include "context.h"
# include "../system/io.h"
# include "pixel.h"
# include "tex.h"
# include "plate.h"
# include "../maths/floor.h"
// need to be worked on but will do for now
/*
	only can rasterize tri
	or any other simple shapes if repurposed
	
	complex shapes that intersect wont work

	this is very simple but fast
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


//#define use_barycentric
#define dot(__x0, __y0, __x1, __y1)\
	((__x0*__x1)+(__y0*__y1))

/*
	not realy barycentric but the method to get it
	might remove and might not work
*/
ff_i8_t static
barycentric(ff_int_t __x, ff_int_t __y, struct nt_vertex2 *__a, struct nt_vertex2 *__b, struct nt_vertex2 *__c) {
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

	float d00, d01, d11, d02, d12;
	d00 = dot(v0.x, v0.y, v0.x, v0.y);
	d01 = dot(v0.x, v0.y, v1.x, v1.y);
	d11 = dot(v1.x, v1.y, v1.x, v1.y);
	d02 = dot(v0.x, v0.y, v2.x, v2.y);
	d12 = dot(v1.x, v1.y, v2.x, v2.y);
	if ((d02*d11-d12*d01) <= 0 && (d12*d00-d02*d01) <= 0 && (d01*d01-d12*d01) <= 0)
		return 0;
	return -1;
}

void nt_raster_tri2(void) {
	struct nt_tri2 *tri;

	tri = (struct nt_tri2*)(nt_raise_stack+(*(ff_u16_t*)nt_raise_p));

	struct nt_tex *tex;

	/*
		texture will be loaded diffrently but for now
	*/
	tex = nt_plate_get(*(ff_u32_t*)(nt_raise_p+2));
	ff_u32_t _x, _y;

	_x = *(ff_u32_t*)(nt_raise_p+6);
	_y = *(ff_u32_t*)(nt_raise_p+10);

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

	ff_uint_t area;
	area = abs((v0.x-v1.x)*(v0.y-v1.y))+abs((v0.x-v2.x)*(v0.y-v2.y));
	ff_uint_t width, height;
	//width = ffly_max(abs(v0.x-v1.x),abs(v0.x-v2.x));
	width = xmax-xmin;
	height = ymax-ymin;

	ff_uint_t loc;
	loc = 0;

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
				ff_uint_t x0 = _x-x;
				ff_uint_t y0 = _y-y;

				tx = x0>>TILESZ;
				ty = y0>>TILESZ;
				if (!(t = *(tp = tile_at(tx, ty, fb)))) {
					t = (*tp = nt_tile_new(TILESZ));
					nt_tile_map(t);
				}	
				txo = x0-(tx<<TILESZ);
				tyo = y0-(ty<<TILESZ);

				loc = (xmax-x)+((ymax-y)*width);
				float a, b;
				a = ((float)(xmax-x))*((float)tex->width)/(float)width;
				b = ((float)(ymax-y))*((float)tex->height)/(float)height;

				loc = ffly_floor(a)+(ffly_floor(b)*tex->width); 
				ff_byte_t *dst;
				dst = tilepx(t, txo, tyo);
		
				nt_setpix(c+(loc*4), dst);
			}
			x++;
		}
		y++;
	}	
}
