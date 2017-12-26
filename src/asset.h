# ifndef __ffly__asset__h
# define __ffly__asset__h
# include "types/id_t.h"
# include <mdlint.h>
# include "types/byte_t.h"
# include "types/mode_t.h"
struct ffly_asset {
	ffly_id_t id;
	mdl_uint_t kind;
	ffly_byte_t *data;
	mdl_uint_t bc;
};

typedef struct ffly_asset* ffly_assetp;

# ifdef __cplusplus
namespace mdl {
namespace firefly {
typedef struct ffly_asset asset;
}
}
# endif
# endif /*__ffly__asset__h*/
