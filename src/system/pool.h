# ifndef __ffly__pool__h
# define __ffly__pool__h
# include "../ffint.h"
# include "../types.h"

//#define FF_POOL_SA

/*
	move to /memory as it has nothing to do with /system
*/
struct ffly_pool {
#ifdef FF_POOL_SA
	void*(*alloc)(long long, ff_uint_t);
	void(*free)(long long, void*);
	void*(realloc)(long long, void*, ff_uint_t);
	long long arg;
#endif
	void *p, *uu_slices;
	ff_size_t size, slice_size;
	ff_off_t off;
	ff_uint_t uu_slice_c;
	ff_mlock_t lock;
};

# ifdef __cplusplus
extern "C" {
# endif
ff_err_t ffly_pool_init(struct ffly_pool*, ff_size_t, ff_size_t);
ff_err_t ffly_pool_de_init(struct ffly_pool*);
void* ffly_pool_get(struct ffly_pool*, ff_uint_t);
void* ffly_pool_alloc(struct ffly_pool*);
ff_err_t ffly_pool_free(struct ffly_pool*, void*);
ff_bool_t ffly_pool_full(struct ffly_pool*);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__pool__h*/
