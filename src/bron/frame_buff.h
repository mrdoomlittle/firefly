# ifndef __ffly__frame__buff__h
# define __ffly__frame__buff__h
# include "../ffint.h"
#define bron_fb_new \
	ffly_bron_fb_new
#define bron_fb_destroy \
	ffly_bron_fb_destroy
#define bron_fb_set \
	ffly_bron_fb_set
ff_u32_t ffly_bron_fb_new(ff_uint_t, ff_uint_t);
void ffly_bron_fb_destroy(ff_u32_t);
void ffly_bron_fb_set(ff_u32_t);
# endif /*__ffly__frame__buff__h*/
