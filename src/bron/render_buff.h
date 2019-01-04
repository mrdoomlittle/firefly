# ifndef __ffly__bron__render__buff__h
# define __ffly__bron__render__buff__h
# include "../ffint.h"
#define bron_rb_bind	ffly_bron_rb_bind
#define bron_rb_new		ffly_bron_rb_new
#define bron_rb_destroy	ffly_bron_rb_destroy
ff_u32_t ffly_bron_rb_new(ff_uint_t, ff_uint_t);
void ffly_bron_rb_destroy(ff_u32_t);
void ffly_bron_rb_bind(ff_u32_t, ff_u32_t);
# endif /*__ffly__bron__render__buff__h*/
