# ifndef __ffly__bron__tile__h
# define __ffly__bron__tile__h
# include "../ffint.h"
#define bron_ptile_new \
	ffly_bron_ptile_new
#define bron_ptile_destroy \
	ffly_bron_ptile_destroy
#define bron_tdraw \
	ffly_bron_tdraw
ff_u32_t ffly_bron_ptile_new(void(*)(ff_u8_t, long long, void*), void*);
void ffly_bron_ptile_destroy(ff_u32_t);
void ffly_bron_tdraw(ff_u32_t, ff_u32_t, ff_u32_t);
# endif /*__ffly__bron__tile__h*/
