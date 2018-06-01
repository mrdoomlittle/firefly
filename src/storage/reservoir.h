# ifndef __ffly_reservoir__h
# define __ffly_reservoir__h
# include "../ffint.h"
# include "../types.h"
# define SLAB_SHIFT 6
# define SLAB_SIZE (1<<SLAB_SHIFT)
typedef struct ffly_slab {
	ff_u64_t off;
	struct ffly_slab *fd;
	struct ffly_slab *prev, *next;
} *ffly_slabp;

typedef struct ffly_reservoir {
	int fd;
	ff_u64_t off;
	ffly_slabp bin, top;
} *ffly_reservoirp;

ff_err_t ffly_reservoir_init(ffly_reservoirp, char const*);
ff_err_t ffly_reservoir_de_init(ffly_reservoirp);

void* ffly_reservoir_alloc(ffly_reservoirp, ff_uint_t);
ff_err_t ffly_reservoir_free(ffly_reservoirp, void*);
ff_err_t ffly_reservoir_write(ffly_reservoirp, void*, void*, ff_uint_t, ff_uint_t);
ff_err_t ffly_reservoir_read(ffly_reservoirp, void*, void*, ff_uint_t, ff_uint_t);
void ffly_reservoir_info(ffly_reservoirp);

void ffly_slab_cleanup(ffly_reservoirp);
ffly_slabp ffly_slab_alloc(ffly_reservoirp);
ff_err_t ffly_slab_free(ffly_reservoirp, ffly_slabp);
ff_err_t ffly_slab_write(ffly_reservoirp, ffly_slabp, void*, ff_uint_t, ff_uint_t);
ff_err_t ffly_slab_read(ffly_reservoirp, ffly_slabp, void*, ff_uint_t, ff_uint_t);
# endif /*__ffly_reservoir__h*/
