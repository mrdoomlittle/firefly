# include "../ffint.h"
# include "raise.h"
# include "context.h"
# include "../system/io.h"
# include "tile.h"
# include "shit.h"
# include "pixel.h"
# include "view.h"
void
nt_pixfill(void) {
	ffly_printf("pixfill.\n");
	ff_u8_t *colour;
	ff_u32_t n;

	n = *(ff_u32_t*)nt_raise_p;
	colour = *(ff_u8_t**)(nt_raise_p+4);
	ffly_printf("fill colour: %u.%u.%u.%u\n", colour[0], colour[1], colour[2], colour[3]);

	struct nt_context *ctx;
	ctx = nt_ctx;

	struct nt_tile *t, **tp;
	ff_uint_t i;

	ff_u8_t *d;
	ff_uint_t tx, ty;
	ff_uint_t txo, tyo;
	ff_uint_t x, y;
	y = 0;
	x = 0;
	i = 0;
	while(i != n) {
		tx = x>>RB_TILESZ;
		ty = y>>RB_TILESZ;

		nwivpgt(x, y, _sk);

		txo = x-(tx<<RB_TILESZ);
		tyo = y-(ty<<RB_TILESZ);

		if (!(t = *(tp = rb_tile_at(tx, ty, ctx->rb)))) {
			t = (*tp = nt_tile_new(RB_TILESZ));
			nt_tile_map(t);
		}
		
		d = tilepx(t, txo, tyo);
		nt_setpix(colour, d);
	_sk:
		if ((i-(y*ctx->rb->width)) >= ctx->rb->width-1) {
			y++;
			x = 0;
		} else {
			x++;
		}
		i++;

	}
}
