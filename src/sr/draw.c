# include "../ffint.h"
# include "raise.h"
# include "context.h"
// needs testing
# include "tile.h"
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
