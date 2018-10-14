# include "../ffint.h"
# include "raise.h"
# include "context.h"
// needs testing
# include "tile.h"
# include "../system/io.h"
# include "plate.h"
# include "shit.h"
# include "pixel.h"
void
nt_tdraw(void) {
	ff_u16_t plate;

	plate = *(ff_u16_t*)nt_raise_p;

	struct nt_ptile *pt;
	pt = (struct nt_ptile*)nt_plate_get(plate);

	ff_u32_t _x, _y;

	_x = *(ff_u32_t*)(nt_raise_p+2);
	_y = *(ff_u32_t*)(nt_raise_p+6);

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

			tx = rx>>TILESZ;
			ty = ry>>TILESZ;

			txo = rx-(tx*(1<<TILESZ));
			tyo = ry-(ty*(1<<TILESZ));

			if (!(t = *(tp = tile_at(tx, ty, ctx->fb)))) {
				t = (*tp = nt_tile_new(TILESZ));
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
			tx = x>>TILESZ;
			ty = y>>TILESZ;

			txo = x-(tx*(1<<TILESZ));
			tyo = y-(ty*(1<<TILESZ));

			if (!(t = *(tp = tile_at(tx, ty, ctx->fb)))) {
				t = (*tp = nt_tile_new(TILESZ));
				nt_tile_map(t);
			}

			d = tilepx(t, txo, tyo);

			nt_setpix(s, d);
			x++;
		}
		y++;
	}
}