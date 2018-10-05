# ifndef __ffly__graphics__slurry__m__h
# define __ffly__graphics__slurry__m__h
# include "../ffint.h"
# include "../slurry/connection.h"
struct ffly_slurry_ctx {
	s_connp conn;
	ff_u8_t *frame_buff;
	ff_u16_t w, title;
	ff_uint_t width, height;
	ff_u16_t d;
};

# endif /*__ffly__graphics__slurry__m__h*/
