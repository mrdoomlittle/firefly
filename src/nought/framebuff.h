# ifndef __ffly__nt__framebuff__h
# define __ffly__nt__framebuff__h
# include "../ffint.h"
# include "tile.h"
# define TILESZ _nt_tile_16
# define tile_at(__x, __y, __fb) \
	((__fb)->tiles+(__x)+((__y)*(__fb)->width))
struct nt_framebuff {
	ff_uint_t width, height;
	ff_uint_t rw, rh;

	struct nt_tile **tiles;
	ff_uint_t n;
};


struct nt_framebuff* nt_framebuff_creat(ff_uint_t, ff_uint_t);
void nt_framebuff_del(struct nt_framebuff*);
void nt_putframe(void);
void nt_fb_set(void);
void nt_fb_new(void);
void nt_fb_destroy(void);
# endif /*__ffly__nt__framebuff__h*/
