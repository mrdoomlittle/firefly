# include "../ffint.h"
# include "raise.h"
# include "context.h"
// needs testing
# include "tile.h"
# include "../system/io.h"
# include "plate.h"
# include "shit.h"
# include "pixel.h"
# include "view.h"
void
nt_tdraw(void) {
	ff_u32_t plate;

	plate = *(ff_u32_t*)nt_raise_p;

	struct nt_ptile *pt;
	pt = (struct nt_ptile*)nt_plate_get(plate);

	ff_u32_t _x, _y;

	_x = *(ff_u32_t*)(nt_raise_p+4);
	_y = *(ff_u32_t*)(nt_raise_p+8);

	ffly_printf("tdraw.\n");	
	ff_u8_t tsz;

	/*
		{pt->get} for now 
	*/

	pt->get(0x02, (long long)&tsz, pt->tile);

	ff_u8_t *ntc, *d, *s;

	pt->get(0x01, (long long)&ntc, pt->tile);

	ff_uint_t tx, ty;
	ff_uint_t txo, tyo;
	struct nt_context *ctx;
	ctx = nt_ctx;

	struct nt_tile *t, **tp;
	ff_uint_t x, y;
	y = 0;
	while(y != (1<<tsz)) {
		x = 0;
		while(x != (1<<tsz)) {
			s = ntc+((x+(y*(1<<tsz)))*4);
			ff_uint_t rx, ry;
			rx = _x+x;
			ry = _y+y;

			tx = rx>>RB_TILESZ;
			ty = ry>>RB_TILESZ;

			txo = rx-(tx<<RB_TILESZ);
			tyo = ry-(ty<<RB_TILESZ);

			if (!(t = *(tp = rb_tile_at(tx, ty, ctx->rb)))) {
				t = (*tp = nt_tile_new(RB_TILESZ));
				nt_tile_map(t);
			}

			d = tilepx(t, txo, tyo);
			nt_setpix(s, d);
			x++;
		}
		y++;
	}
}

void
nt_pixdraw(void) {
	ff_u32_t _x, _y;
	ff_u8_t *pixels;
	ff_uint_t width, height;

	_x = *(ff_u32_t*)nt_raise_p;
	_y = *(ff_u32_t*)(nt_raise_p+4);
	pixels = *(ff_u8_t**)(nt_raise_p+8);
	width = *(ff_u32_t*)(nt_raise_p+16);
	height = *(ff_u32_t*)(nt_raise_p+20);

	struct nt_tile *t, **tp;
	struct nt_context *ctx;
	ctx = nt_ctx;

	ff_uint_t tx, ty;
	ff_uint_t txo, tyo;

	ff_u8_t *s, *d;
	ff_uint_t x, y;
	y = _y;

	while(y != _y+height) {
		x = _x;
		while(x != _x+width) {
			s = (pixels+(((x-_x)+((y-_y)*width))*4));
			tx = x>>RB_TILESZ;
			ty = y>>RB_TILESZ;

			txo = x-(tx<<RB_TILESZ);
			tyo = y-(ty<<RB_TILESZ);

			if (!(t = *(tp = rb_tile_at(tx, ty, ctx->rb)))) {
				t = (*tp = nt_tile_new(RB_TILESZ));
				nt_tile_map(t);
			}

			d = tilepx(t, txo, tyo);

			nt_setpix(s, d);
			x++;
		}
		y++;
	}
}

# include "objbuf.h"

ff_i8_t static
within(struct nt_vertex2 *__v, ff_int_t __x, ff_int_t __y, ff_uint_t __n) {
	ff_uint_t i;
	struct nt_vertex2 *v0, *v1;
	ff_int_t t0, t1, t2, t3;
	t0 = 0;
	t1 = 0;
	t2 = 0;
	t3 = 0;
	i = 0;

	while(i != __n-1) {
		v0 = __v+(i++);
		v1 = __v+i;

		ff_int_t x0, y0;
		ff_int_t x, y;

		x0 = v1->x-v0->x;
		y0 = v1->y-v0->y;

		x = __x-v0->x;
		y = __y-v0->y;

		ff_int_t k0, k1;
		k0 = x*y0;
		k1 = x0*y;

		ff_int_t s;
		s = k0-k1;

		ff_int_t b0, b1, b2, b3;
		b0 = __x-v0->x;
		b1 = __x-v1->x;
		b2 = __y-v0->y;
		b3 = __y-v1->y;

		if ((b2>=0 && b3<=0) || (b2<=0 && b3>=0)) {
			t0+=s>0;
			t1+=s<0;
		}

		if ((b0>=0 && b1<=0) || (b0<=0 && b1>=0)) {
			t2+=s>0;
			t3+=s<0;
		}
	}
	if (t0-t1<0 && t2-t3<0) {
		return 0;
	}
	if (t0-t1>0 && t2-t3>0) {
		return 0;
	}
	return -1;
}

void
nt_draw(void) {
	nt_objbufp buf;
	buf = (nt_objbufp)nt_plate_get(*(ff_u32_t*)nt_raise_p);	

	ff_u32_t n;
	n = *(ff_u32_t*)(nt_raise_p+4);

	struct nt_vertex2 *v, *cr;
	v = (struct nt_vertex2*)buf->data;

	ff_uint_t mnx, mny;
	ff_uint_t mxx, mxy;

	mnx = v->x;
	mny = v->y;
	mxx = v->x;
	mxy = v->y;

	ff_uint_t i;
	i = 0;
	while(i+1 != n) {
		cr = v+i+1;
		if (cr->x>mxx)
			mxx = cr->x;
		else if (cr->x<mnx) 
			mnx = cr->x;
		if (cr->y>mxy)
			mxy = cr->y;
		else if (cr->y<mny)
			mny = cr->y;
		i++;
	}

	ffly_printf("draw. min{%u, %u}, max{%u, %u}\n", mnx, mny, mxx, mxy);
	ff_uint_t x, y;
	ff_uint_t tx, ty;
	ff_uint_t txo, tyo;
	struct nt_tile *t, **tp;

	ff_u8_t tex[4] = {0, 255, 0, 255};
	struct nt_renderbuff *rb;

	rb = nt_ctx->rb;

	y = mny;
	while(y != mxy) {
		x = mnx;
		while(x != mxx) {
			ff_i8_t rs;
			rs = within(v, x, y, n);
			if (!rs) {
				tx = x>>RB_TILESZ;
				ty = y>>RB_TILESZ;
				nwivpgt(x, y, _sk);
				if (!(t = *(tp = rb_tile_at(tx, ty, rb)))) {
					t = (*tp = nt_tile_new(RB_TILESZ));
					nt_tile_map(t);
				}

				txo = x-(tx<<RB_TILESZ);
				tyo = y-(ty<<RB_TILESZ);

				ff_byte_t *dst;
				dst = tilepx(t, txo, tyo);
				if (rs == -2) {
					*tex = 255;
				}
				nt_setpix(tex, dst);
				if (rs == -2) {
					*tex = 0;
				}
			}
		_sk:
			x++;
		}
		y++;
	}
}
