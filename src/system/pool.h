# ifndef __ffly__pool__h
# define __ffly__pool__h
# include <mdlint.h>
# include "../types/err_t.h"
# include "../types/bool_t.h"
# include "../types/mutex_t.h"
# include "../types/off_t.h"
# include "../types/size_t.h"

// move to memory
struct ffly_pool {
	void *p, *uu_slices;
	ffly_size_t size, slice_size;
	ffly_off_t off;
	mdl_uint_t uu_slice_c;
	ffly_mutex_t m;
};

# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_pool_init(struct ffly_pool*, ffly_size_t, ffly_size_t);
ffly_err_t ffly_pool_de_init(struct ffly_pool*);
void* ffly_pool_get(struct ffly_pool*, mdl_uint_t);
void* ffly_pool_alloc(struct ffly_pool*);
ffly_err_t ffly_pool_free(struct ffly_pool*, void*);
ffly_bool_t ffly_pool_full(struct ffly_pool*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__pool__h*/
