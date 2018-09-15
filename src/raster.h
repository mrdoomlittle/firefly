# ifndef __ffly__raster__h
# define __ffly__raster__h
# include "ffint.h"
# include "types.h"
# include "model.h"
# include "rosin.h"
# include "tape.h"


struct rcontext {
	ff_u8_t *dst;
	ff_uint_t width, height;
	ff_uint_t x, y;
	void *tex;
};

void ffly_rraster(struct rr_struc*);
void ffly_rasterize(ffly_modelp, ff_byte_t*, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t);
# endif /*__ffly__raste__h*/
