# ifndef __ffly__tile__h
# define __ffly__tile__h
# include "ffint.h"
# include "types.h"

# define ffly_tile_set(__tile, __p) \
	(__tile)->p = __p;
enum {
	_ffly_tile_8 = 3,
	_ffly_tile_16,
	_ffly_tile_32,
	_ffly_tile_64
};

typedef struct ffly_tile {
	ff_u8_t size;
	ff_u8_t *p;
	ff_u16_t xoff, yoff;
	struct ffly_tile *child;
	struct ffly_tile **bk, *next;
	ff_i8_t blank;
} *ffly_tilep;


void ffly_tile_draw(ffly_tilep, ff_u8_t*, ff_u16_t, ff_u16_t, ff_u16_t, ff_u16_t);
ffly_tilep ffly_tile_creat(ff_u8_t);
void ffly_tile_del(ffly_tilep);
void ffly_tile_cleanup(void);
# endif /*__ffly__tile__h*/
