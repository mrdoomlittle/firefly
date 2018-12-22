# ifndef __ffly__gframe__buff__h
# define __ffly__gframe__buff__h
# include "../ffint.h"
# include "../types.h"
# include "../tile.h"
#define ffly_frame(__fb) \
	((__fb)->frame)
#define fb_creat(...)\
	ffly_frame_buff_creat(__VA_ARGS__)
#define fb_del(...)\
	ffly_frame_buff_del(__VA_ARGS__)
#define fb_w(__fb)\
	(__fb)->width
#define fb_h(__fb)\
	(__fb)->height
#define fb_cc(__fb)\
	(__fb)->chn_c
#define fb_gen(__fb)\
	ffly_fb_gen(__fb)
#define plfb(__fb)\
	__ctx(hl_fb) = __fb
/*
*   we dont want to directly use the window frame buffer,
*   as we want to be able to pipe the frame over to some where else if needed.
*/

typedef struct ffly_frame_buff {
	ff_uint_t width, height;
	ff_u8_t chn_c;
	ff_byte_t *frame;
	ffly_tilep *tiles;
	ff_uint_t wt, ht, n;
} *ffly_frame_buffp;



ffly_frame_buffp ffly_frame_buff_creat(ff_uint_t, ff_uint_t, ff_u8_t, ff_err_t*);
ff_err_t ffly_frame_buff_del(ffly_frame_buffp);
ff_err_t ffly_frame_buff_init(ffly_frame_buffp, ff_uint_t, ff_uint_t, ff_u8_t);
ff_err_t ffly_frame_buff_de_init(ffly_frame_buffp);
void ffly_fb_gen(ffly_frame_buffp);
void ffly_fb_read(ffly_frame_buffp, void*, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t);
void ffly_fb_write(ffly_frame_buffp, void*, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t);
void ffly_fb_yank(ffly_frame_buffp);
void ffly_fb_copy(ffly_frame_buffp);
# endif /*__ffly__gframe__buff__h*/
