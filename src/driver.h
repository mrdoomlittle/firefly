# ifndef __ffly__driver__h
# define __ffly__driver__h
# include "ffint.h"
# include "types.h"
struct ff_driver {
	ff_err_t(*pixcopy)(ff_byte_t*, ff_byte_t*, ff_uint_t);
	ff_err_t(*pixdraw)(ff_uint_t, ff_uint_t, ff_byte_t*, ff_uint_t, ff_byte_t*, ff_uint_t, ff_uint_t);
	ff_err_t(*pixfill)(ff_byte_t*, ff_uint_t, ffly_colour_t);
};

# endif /*__ffly__driver__h*/
