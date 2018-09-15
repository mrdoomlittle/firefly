# include "../ffint.h"
# include "raise.h"
# include "context.h"
# include "../system/io.h"
# include "tile.h"
void
sr_pixfill(void) {
	ffly_printf("pixfill.\n");

	ff_u8_t *colour;
	ff_u32_t n;

	n = *(ff_u32_t*)sr_raise_p;
	colour = *(ff_u8_t**)(sr_raise_p+4);

	struct sr_context *ctx;
	ctx = sr_ctx;

	struct sr_tile *t, **tp;
	ff_uint_t i;

	ff_uint_t tx, ty;
	ff_uint_t txo, tyo;
	ff_uint_t x, y;
	y = 0;
	x = 0;
	i = 0;
	while(i != n) {
		tx = x>>TILESZ;
		ty = y>>TILESZ;

		txo = x-(tx*(1<<TILESZ));
		tyo = y-(ty*(1<<TILESZ));

		if (!(t = *(tp = tile_at(tx, ty, ctx->fb)))) {
			t = (*tp = sr_tile_new(TILESZ));
			sr_tile_map(t);
		}

		*(ff_u32_t*)tilepx(t, txo, tyo) = *(ff_u32_t*)colour;
		
		if ((i-(y*ctx->fb->rw)) >= ctx->fb->rw-1) {
			y++;
			x = 0;
		} else {
			x++;
		}
		i++;

	}
}
