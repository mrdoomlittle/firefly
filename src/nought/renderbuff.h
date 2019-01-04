# ifndef __ffly__nt__renderbuff__h
# define __ffly__nt__renderbuff__h
# include "../ffint.h"
# include "tile.h"
# include "framebuff.h"
#define RB_TILESZ _nt_tile_16
#define rb_tile_at(__x, __y, __rb) \
    ((__rb)->tiles+(__x)+((__y)*(__rb)->tw))
struct nt_renderbuff {
	ff_uint_t width, height;
	// width & height in tiles
	ff_uint_t tw, th;

	struct nt_tile **tiles;
	// num of tiles
	ff_uint_t n;

	/*
		where finished render will end up
	*/	
	struct nt_framebuff *dst;

	struct nt_renderbuff **bk, *next;
};


struct nt_renderbuff* nt_renderbuff_creat(ff_uint_t, ff_uint_t);
void nt_renderbuff_del(struct nt_renderbuff*);
void nt_rb_bind(void);
void nt_rb_new(void);
void nt_rb_destroy(void);
# endif /*__ffly__nt__renderbuff__h*/
