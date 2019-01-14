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
# include "depth.h"
# include "slagprog.h"
// need to be worked on but will do for now
/*
	only can rasterize tri
	or any other simple shapes if repurposed
	
	complex shapes that intersect wont work

	this is very simple but fast
*/

// using this for now
static ff_s32_t x0, y0, z0;
static ff_s32_t x1, y1, z1;
static ff_s32_t x2, y2, z2;

ff_i8_t
static dim(ff_int_t __x, ff_int_t __y) {
	struct nt_vertex2 v0 = {
		.x=x1-x0,
		.y=y1-y0
	};

	struct nt_vertex2 v1 = {
		.x=x2-x0,
		.y=y2-y0
	};

	struct nt_vertex2 v2 = {
		.x=__x-x0,
		.y=__y-y0
	};

	float x, y;
	x = v2.x;
	y = v2.y;

	float f0, f1, f2, f3, f4, f5;

	f0 = x*(y2-y0);
	f1 = (x2-x0)*y;

	f2 = x*(y0-y1);
	f3 = (x0-x1)*y;

	f4 = (v1.x-x)*(y2-y1);
	f5 = (x2-x1)*(v1.y-y);

	if ((f0-f1)*(f2-f3) >= 0 && (f2-f3)*(f4-f5) >= 0 && (f0-f1)*(f4-f5) >= 0)
		return 0;
	return -1;
}

/*
	TODO:
		change dot

		to __x0, __x1, __y0, __y1
*/
//#define use_barycentric
#define dot(__x0, __y0, __x1, __y1)\
	((__x0*__x1)+(__y0*__y1))

/*
	not realy barycentric but the method to get it
	might remove and might not work i just thought i should have it here
*/
ff_i8_t static
bary(ff_int_t __x, ff_int_t __y) {
	struct nt_vertex2 v0 = {
		.x=x1-x0,
		.y=y1-y0
	};

	struct nt_vertex2 v1 = {
		.x=x2-x0,
		.y=y2-y0
	};

	struct nt_vertex2 v2 = {
		.x=__x-x0,
		.y=__y-y0
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

static ff_uint_t nds(char *__buf, ff_uint_t __num) {
	char *p;
	p = __buf;
	if (!__num) {
		*__buf = '0';
		return 1;
	}
 
	ff_uint_t pl, n = 0;
	pl = 1;
	for(;pl*10<__num;pl*=10,n++);

	ff_uint_t i;
	i = 0;
	while(i != n+1) {
		ff_uint_t val;
		val = __num/pl;
		*(p++) = '0'+val;
		__num-=val*pl;
		pl/=10;
		i++;
	}

	return i;
}

static ff_u32_t _x, _y, _z;
static struct nt_tex *tex;
# include "text.h"
void static
raster_tri(void) {
	ff_int_t xmin = ffly_min3(x0, x1, x2);
	ff_int_t ymin = ffly_min3(y0, y1, y2);

	ff_int_t xmax = ffly_max3(x0, x1, x2);
	ff_int_t ymax = ffly_max3(y0, y1, y2);
	ffly_printf("xmin: %d, ymin: %d, xmax: %d, ymax: %d\n", xmin, ymin, xmax, ymax);

	struct nt_renderbuff *rb;

	rb = nt_ctx->rb;

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
	area = abs((x0-x1)*(y0-y1))+abs((x0-x2)*(y0-y2));
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
			if (!dim(x, y)) {
#endif
#ifdef use_barycentric
			if (a>=0 && b>=0 && g>=0) {
#endif
				ff_uint_t _x0 = _x-x;
				ff_uint_t _y0 = _y-y;

				/*
					if not in viewport then dont draw
				*/
				nwivpgt(_x0, _y0, _sk);

				tx = _x0>>RB_TILESZ;
				ty = _y0>>RB_TILESZ;
				if (!(t = *(tp = rb_tile_at(tx, ty, rb)))) {
					t = (*tp = nt_tile_new(RB_TILESZ));
					nt_tile_map(t);
				}	
				txo = _x0-(tx<<RB_TILESZ);
				tyo = _y0-(ty<<RB_TILESZ);

				loc = (xmax-x)+((ymax-y)*width);
				float a, b;
				a = ((float)(xmax-x))*((float)tex->width)/(float)width;
				b = ((float)(ymax-y))*((float)tex->height)/(float)height;

				loc = ffly_floor(a)+(ffly_floor(b)*tex->width); 
				ff_byte_t *dst;
				dst = tilepx(t, txo, tyo);

				double a0, a1, a2, a3, a4, a5;
				double b0, b1, b2, b3, b4, b5, b6, b7, b8;
				double d0, d1;
				a0 = x1-x0;
				a1 = y1-y0;
				a2 = x2-x0;
				a3 = y2-y0;
				a4 = x-x0;
				a5 = y-y0;
				d0 = dot(a0, a1, a0, a1);
				d1 = dot(a2, a3, a2, a3);
/*
	may not work on one side of tri idk i dont care this will do for now
*/

				b3 = ((float)z0)/d0;
				b0 = (d0-dot(a4, a5, a0, a1))*b3;
				b4 = b0/d1;
				b0 = (d1-dot(a4, a5, a2, a3))*b4;

				b5 = ((float)z1)/d0;
				b1 = (dot(a4, a5, a0, a1))*b5;
				b6 = b1/d1;
				b1 = (dot(a4, a5, a2, a3))*b6;

				b7 = ((float)z2)/d0;
				b2 = (d0-dot(a4, a5, a0, a1))*b7;
				b8 = b2/d1;
				b2 = (dot(a4, a5, a2, a3))*b8;

				float z;
				z = abs(b0)+abs(b1)+abs(b2);
				z *= 100;//higher the value the more precision as to push fraction bits into a whole numer
		//		ffly_printf("%u, ", (ff_u32_t)z);
				if (!nt_dpbtas((ff_u16_t)z, _x0, _y0)) {
					slg.px = *(ff_u32_t*)(c+(loc*4));
					slg.z = (ff_u16_t)z;
					nt_slags_exec();
					nt_setpix((ff_u8_t*)&slg.px, dst);
				}

			}
		_sk:
			x++;
		}
		//ffly_printf("\n");
		y++;
	}
	// might change this
	if (((ff_s32_t)(_x-x0))>0 && ((ff_s32_t)(_y-y0))>0 && ((ff_s32_t)(_x-x1))>0 && ((ff_s32_t)(_y-y1))>0 && ((ff_s32_t)(_x-x2))>0 && ((ff_s32_t)(_y-y2))>0) {
		char buf[128];
		ff_uint_t n;
		nt_text(_x-x0, _y-y0, "0");
		nt_text(_x-x1, _y-y1, "1");
		nt_text(_x-x2, _y-y2, "2");
	
		n = nds(buf, z0);
		buf[n] = '\0';
		nt_text(_x-x0, (_y-y0)+8, buf);
	
		n = nds(buf, z1);
		buf[n] = '\0';
		nt_text(_x-x1, (_y-y1)+8, buf);
	
		n = nds(buf, z2);
		buf[n] = '\0';
		nt_text(_x-x2, (_y-y2)+8, buf);
	}
}

void nt_raster_tri3(void) {
	struct nt_tri3 *tri;
	tri = (struct nt_tri3*)(nt_raise_stack+(*(ff_u16_t*)nt_raise_p));

	tex = nt_plate_get(*(ff_u32_t*)(nt_raise_p+2));
	_x = *(ff_u32_t*)(nt_raise_p+6);
	_y = *(ff_u32_t*)(nt_raise_p+10);
	_z = *(ff_u32_t*)(nt_raise_p+14);

	x0 = tri->x0;
	y0 = tri->y0;
	z0 = tri->z0;
	x1 = tri->x1;
	y1 = tri->y1;
	z1 = tri->z1;
	x2 = tri->x2;
	y2 = tri->y2;
	z2 = tri->z2;
	

	ffly_printf("0(x: %d, y: %d, z: %d),\n1(x: %d, y: %d, z: %d),\n2(x: %d, y: %d, z: %d),\n", x0, y0, z0, x1, y1, z1, x2, y2, z2);
	raster_tri();
}

void nt_raster_tri2(void) {
	struct nt_tri2 *tri;

	tri = (struct nt_tri2*)(nt_raise_stack+(*(ff_u16_t*)nt_raise_p));

	/*
		texture will be loaded diffrently but for now
	*/
	tex = nt_plate_get(*(ff_u32_t*)(nt_raise_p+2));

	_x = *(ff_u32_t*)(nt_raise_p+6);
	_y = *(ff_u32_t*)(nt_raise_p+10);
	_z = 0;

	ffly_printf("--------> %ux%u\n", _x, _y);
	struct nt_vertex2 v0, v1, v2;

	v0 = tri->v0;
	v1 = tri->v1;
	v2 = tri->v2;

	ffly_printf("v0: %d, %d\nv1: %d, %d\nv2: %d, %d\n", v0.x, v0.y, v1.x, v1.y, v2.x, v2.y);

	x0 = v0.x;
	y0 = v0.y;
	x1 = v1.x;
	y1 = v1.y;
	x2 = v2.x;
	y2 = v2.y;

	z0 = 0;
	z1 = 0;
	z2 = 0;
	raster_tri();
}
