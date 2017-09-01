# ifndef __chunk__info__h
# define __chunk__info__h
# include "types/coords_t.h"
# include "types/id_t.h"
struct ffly_chunk_info {
	ffly_3d_coords_t coords;
	ffly_id_t id;
};

# ifdef __cplusplus
namespace mdl {
namespace firefly {
typedef struct ffly_chunk_info chunk_info;
}
}
# endif
# endif /*__chunk__info__h*/
