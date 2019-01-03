# ifndef __ffly__bron__objbuf__h
# define __ffly__bron__objbuf__h
# include "../ffint.h"
#define bron_objbuf_new \
	ffly_bron_objbuf_new
#define bron_objbuf_destroy \
	ffly_bron_objbuf_destroy
ff_u32_t ffly_bron_objbuf_new(ff_u32_t);
void ffly_bron_objbuf_destroy(ff_u32_t);
void ffly_bron_objbuf_map(ff_u32_t);
void ffly_bron_objbuf_unmap(ff_u32_t);
void ffly_bron_objbuf_write(ff_u32_t, ff_u32_t, ff_u32_t, void*);
void ffly_bron_objbuf_read(ff_u32_t, ff_u32_t, ff_u32_t, void*);
# endif /*__ffly__bron__objbuf__h*/
