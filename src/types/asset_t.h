# ifndef __ffly__asset__t__h
# define __ffly__asset__t__h
# include "id_t.h"
# include <mdlint.h>
# include "byte_t.h"
typedef struct {
	ffly_id_t id;
	mdl_uint_t kind;
	ffly_byte_t *data;
} ffly_asset_t;

# ifdef __cplusplus
namespace mdl {
namespace firefly {
namespace types {
typedef ffly_asset_t asset_t;
}
}
}
# endif
# endif /*__ffly__asset__t__h*/
