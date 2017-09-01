# ifndef __chunk__data__h
# define __chunk__data__h
# include "types/pixmap_t.h"
# include "types/uni_par_t.h"
struct ffly_chunk_data {
	ffly_uni_par_t *particles;
	ffly_1d_pm_t _1d_pm;
	ffly_3d_pm_t _3d_pm;
};

# ifdef __cplusplus
namespace mdl {
namespace firefly {
typedef struct ffly_chunk_data chunk_data;
}
}
# endif
# endif /*__chunk__data__h*/
