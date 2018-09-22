# ifndef __ffly__graphics__h
# define __ffly__graphics__h
# include "ffint.h"
# include "bron/driver.h"
# include "bron/context.h"
# include "bron/tri.h"
# include "bron/tex.h"
# include "bron/dc.h"
# include "bron/prim.h"
# include "bron/pixel.h"
# include "bron/frame_buff.h"
void ff_graphics_init();
void ff_graphics_de_init();
void ff_set_frame_size(ff_uint_t, ff_uint_t);
# endif /*__ffly__graphics__h*/
