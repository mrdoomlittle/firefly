# ifndef __ffly__sr__framebuff__h
# define __ffly__sr__framebuff__h
# include "../ffint.h"
# include "tile.h"
# define TILESZ _sr_tile_16
# define tile_at(__x, __y, __fb) \
	((__fb)->tiles+(__x)+((__y)*(__fb)->width))
struct sr_framebuff {
	ff_uint_t width, height;
	ff_uint_t rw, rh;

	struct sr_tile **tiles;
	ff_uint_t n;
};


struct sr_framebuff* sr_framebuff_creat(ff_uint_t, ff_uint_t);
void sr_framebuff_del(struct sr_framebuff*);
void sr_putframe(void);
void sr_fb_set(void);
void sr_fb_new(void);
void sr_fb_destroy(void);
# endif /*__ffly__sr__framebuff__h*/
