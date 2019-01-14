# ifndef __ffly__bron__view__h
# define __ffly__bron__view__h
# include "../ffint.h"
struct bron_viewport {
	ff_u32_t x, y;
	ff_u32_t width, height;
};
void bron_viewport(struct bron_viewport*);
void bron_lookat(ff_u32_t, ff_u32_t, ff_u32_t, float, float, float);
# endif /*__ffly__bron__view__h*/
