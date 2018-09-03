# ifndef __ffly__typo__raster__h
# define __ffly__typo__raster__h
# include "../typo.h"
extern ff_u8_t *typo_raster_bm;
ff_uint_t extern typo_raster_width;
ff_uint_t extern typo_raster_height;
ff_u8_t extern typo_raster_scale;
void ffly_typo_raster(void(**)(void));
# endif /*__ffly__typo__raster__h*/
