# ifndef __ffly__mem__blk__h
# define __ffly__mem__blk__h
# include <mdlint.h>
# include "../types/err_t.h"
# include "../types/bool_t.h"
# include "../types/mutex_t.h"
# include "../types/off_t.h"
# include "../types/size_t.h"
struct ffly_mem_blk {
	void *p, *uu_slices;
	ffly_size_t size, slice_size;
	ffly_off_t off;
	mdl_uint_t uu_slice_c;
	ffly_mutex_t m;
};

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_mem_blk_init(struct ffly_mem_blk*, ffly_size_t, ffly_size_t);
ffly_err_t ffly_mem_blk_de_init(struct ffly_mem_blk*);
void* ffly_mem_blk_get(struct ffly_mem_blk*, mdl_uint_t);
void* ffly_mem_blk_alloc(struct ffly_mem_blk*);
ffly_err_t ffly_mem_blk_free(struct ffly_mem_blk*, void*);
ffly_bool_t ffly_mem_blk_full(struct ffly_mem_blk*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__mem__blk__h*/
