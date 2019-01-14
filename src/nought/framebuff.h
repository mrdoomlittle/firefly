# ifndef __ffly__nt__framebuff__h
# define __ffly__nt__framebuff__h
# include "../ffint.h"
# include "tile.h"
# define TILESZ _nt_tile_16
#define tile_at(__x, __y, __fb) \
	((__fb)->tiles+(__x)+((__y)*(__fb)->tw))
struct nt_framebuff {
	// TODO: swap around and use tw like renderbuff
	ff_uint_t width, height;
	ff_uint_t tw, th;

	struct nt_tile **tiles;
	ff_uint_t n;

/*
	im not using floats and its just compare and for better pres 0.01*100 = 1
	dk 
*/
	ff_u16_t *dpb;
};


struct nt_framebuff* nt_framebuff_creat(ff_uint_t, ff_uint_t);
void nt_framebuff_del(struct nt_framebuff*);
void nt_putframe(void);
void nt_fb_set(void);
void nt_fb_new(void);
void nt_fb_destroy(void);
# endif /*__ffly__nt__framebuff__h*/
