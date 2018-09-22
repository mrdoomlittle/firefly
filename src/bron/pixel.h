# ifndef __ffly__bron__pixel__h
# define __ffly__bron__pixel__h
# include "../ffint.h"
# define bron_pixdraw \
	ffly_bron_pixdraw
# define bron_pixfill \
	ffly_bron_pixfill
void ffly_bron_pixdraw(ff_u32_t, ff_u32_t, ff_u8_t*, ff_u32_t, ff_u32_t);
void ffly_bron_pixfill(ff_u32_t, ff_u8_t*);
# endif /*__ffly__bron__pixel__h*/
