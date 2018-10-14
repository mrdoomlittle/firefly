# ifndef __ffly__tile__h
# define __ffly__tile__h
# include "ffint.h"
# include "types.h"
# include "mo.h"
# include "pallet.h"
# include "bron/tile.h"
# define ffly_tile_set(__tile, __p) \
	(__tile)->p = __p;

#define TILE_BLANK 0x01
#define TILE_PH 0x02
typedef struct ffly_pallet* ffly_palletp;

enum {
	_ffly_tile_8 = 3,
	_ffly_tile_16,
	_ffly_tile_32,
	_ffly_tile_64
};

typedef struct ffly_tile {
	ff_u8_t size;
	// memory object
	ffly_mop m;
	struct ffly_tile **bk, *next;
	ff_u8_t bits;
	// phantom tile
	ff_u16_t pt;
} *ffly_tilep;

void ffly_tile_draw(ffly_tilep, ff_u32_t, ff_u32_t);
ffly_tilep ffly_tile_creat(ff_u8_t);
void ffly_tile_del(ffly_tilep);
void ffly_tile_cleanup(void);
void ffly_tiles_sched(void);
void ffly_tiles_usched(void);
# endif /*__ffly__tile__h*/
