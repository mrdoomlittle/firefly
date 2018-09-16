# ifndef __ffly__pallet__h
# define __ffly__pallet__h
# include "ffint.h"
# include "types.h"
# include "tile.h"

typedef struct ffly_tile* ffly_tilep;
typedef struct ffly_pallet {
	ff_uint_t width, height;
	ff_uint_t wt, ht;
	
	ffly_tilep *tiles;
	ff_u8_t tilesize;
} *ffly_palletp;

ffly_tilep ffly_tile_at(ffly_palletp, ff_uint_t, ff_uint_t);
ffly_palletp ffly_pallet_new(ff_uint_t, ff_uint_t, ff_u8_t);
void ffly_pallet_distroy(ffly_palletp);

void ffly_pallet_init(ffly_palletp, ff_uint_t, ff_uint_t, ff_u8_t);
void ffly_pallet_de_init(ffly_palletp);
void ffly_pallet_write(ffly_palletp, void*, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t);
void ffly_pallet_draw(ffly_palletp, ff_uint_t, ff_uint_t);
void ffly_pallet_read(ffly_palletp, void*, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t);
# endif /*__ffly__pallet__h*/
