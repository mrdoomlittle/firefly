# include "alloc_pixelmap.h"
# include "mem_alloc.h"
ffly_1d_pm_t ffly_alloc_pixelmap(ff_uint_t __xa_len, ff_uint_t __ya_len, ff_uint_t __za_len, ff_u8_t __chn_c) {
	return (ffly_1d_pm_t)__ffly_mem_alloc( (__xa_len*__ya_len*__za_len*__chn_c)*sizeof(__ffly_pixelmap_t));
}
