# ifndef __ffly__bron__tex__h
# define __ffly__bron__tex__h
# include "../ffint.h"
# include "prim.h"

ff_u16_t ffly_bron_texbuf_new(ff_u32_t);
void ffly_bron_texbuf_destroy(ff_u16_t);
void ffly_bron_texbuf_map(ff_u16_t);
void ffly_bron_texbuf_unmap(ff_u16_t);
void ffly_bron_texbuf_write(ff_u16_t, ff_u32_t, ff_u32_t, void*);
void ffly_bron_texbuf_read(ff_u16_t, ff_u32_t, ff_u32_t, void*);

ff_u16_t ffly_bron_tex_new(ff_u16_t);
# endif /*__ffly__bron__tex__h*/
