# ifndef __ffly__asset__manager__h
# define __ffly__asset__manager__h
# include "types/id_t.h"
# include "types/byte_t.h"
# include "types/asset_t.h"
# include <mdlint.h>
ffly_id_t ffly_load_asset(void*, char*, mdl_uint_t);
ffly_id_t ffly_add_asset(void*, ffly_byte_t*, mdl_uint_t, mdl_uint_t);
ffly_asset_t* ffly_asset(void*, ffly_id_t);
# endif /*__ffly__asset__manager__h*/
