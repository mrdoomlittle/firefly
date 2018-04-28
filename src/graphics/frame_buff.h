# ifndef __ffly__frame__buff__h
# define __ffly__frame__buff__h
# include "../ffint.h"
# include "../types.h"
# define ffly_frame(__fb) \
	((__fb)->frame)
/*
*   we dont want to directly use the window frame buffer,
*   as we want to be able to pipe the frame over to some where else if needed.
*/

typedef struct ffly_frame_buff {
	ff_uint_t width, height;

	ff_byte_t *frame;
} *ffly_frame_buffp;

ffly_frame_buffp ffly_frame_buff_creat(ff_uint_t, ff_uint_t, ff_u8_t, ff_err_t*);
ff_err_t ffly_frame_buff_del(ffly_frame_buffp);
ff_err_t ffly_frame_buff_init(ffly_frame_buffp, ff_uint_t, ff_uint_t, ff_u8_t);
ff_err_t ffly_frame_buff_de_init(ffly_frame_buffp);
# endif /*__ffly__frame__buff__h*/
