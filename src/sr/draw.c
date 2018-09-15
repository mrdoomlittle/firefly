# include "../ffint.h"
# include "raise.h"
# include "context.h"
// needs testing
# include "tile.h"
# include "../system/io.h"
void
sr_tdraw(void) {
	struct sr_ptile *pt;

	pt = *(struct sr_ptile**)stack_at(*(ff_u16_t*)sr_raise_p);
	ff_u32_t _x, _y;

	_x = *(ff_u32_t*)(sr_raise_p+2);
	_y = *(ff_u32_t*)(sr_raise_p+6);

	ffly_printf("tdraw.\n");	
	ff_u8_t tsz;

	/*
		{pt->get} for now 
	*/
	pt->get(0x02, (long long)&tsz, pt->tile);

	ff_u8_t *src, *d, *s;

	pt->get(0x01, (long long)&src, pt->tile);

	ff_uint_t tx, ty;
	ff_uint_t txo, tyo;
	struct sr_context *ctx;
	ctx = sr_ctx;


	struct sr_tile *t, **tp;
	ff_uint_t x, y;
	y = 0;
	while(y != (1<<tsz)) {
		x = 0;
		while(x != (1<<tsz)) {
			s = src+((x+(y*(1<<tsz)))*4);
			ff_uint_t rx, ry;
			rx = _x+x;
			ry = _y+y;

			tx = rx>>TILESZ;
			ty = ry>>TILESZ;

			txo = rx-(tx*(1<<TILESZ));
			tyo = ry-(ty*(1<<TILESZ));

			if (!(t = *(tp = tile_at(tx, ty, ctx->fb)))) {
				t = (*tp = sr_tile_new(TILESZ));
				sr_tile_map(t);
			}

			d = tilepx(t, txo, tyo);
			*(ff_u32_t*)d = *(ff_u32_t*)s;

			x++;
		}
		y++;
	}
}

void
sr_pixdraw(void) {
	ff_u32_t _x, _y;
	ff_u8_t *pixels;
	ff_uint_t width, height;

	_x = *(ff_u32_t*)sr_raise_p;
	_y = *(ff_u32_t*)(sr_raise_p+4);
	pixels = *(ff_u8_t**)(sr_raise_p+8);
	width = *(ff_u32_t*)(sr_raise_p+16);
	height = *(ff_u32_t*)(sr_raise_p+20);

	struct sr_tile *t, **tp;
	struct sr_context *ctx;
	ctx = sr_ctx;

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
				t = (*tp = sr_tile_new(TILESZ));
				sr_tile_map(t);
			}

			d = tilepx(t, txo, tyo);

			*(ff_u32_t*)d = *(ff_u32_t*)s;
			x++;
		}
		y++;
	}
}
