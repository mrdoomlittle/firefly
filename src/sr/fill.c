# include "../ffint.h"
# include "raise.h"
# include "context.h"
# include "../system/io.h"
# include "tile.h"
# include "shit.h"
# include "pixel.h"
void
sr_pixfill(void) {
	ffly_printf("pixfill.\n");

	ff_u8_t *colour;
	ff_u32_t n;

	n = *(ff_u32_t*)sr_raise_p;
	colour = *(ff_u8_t**)(sr_raise_p+4);
	ffly_printf("fill colour: %u.%u.%u.%u\n", colour[0], colour[1], colour[2], colour[3]);

	struct sr_context *ctx;
	ctx = sr_ctx;

	struct sr_tile *t, **tp;
	ff_uint_t i;

	ff_u8_t *d;
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

		
		d = tilepx(t, txo, tyo);
		sr_setpix(colour, d);

		if ((i-(y*ctx->fb->rw)) >= ctx->fb->rw-1) {
			y++;
			x = 0;
		} else {
			x++;
		}
		i++;

	}
}
