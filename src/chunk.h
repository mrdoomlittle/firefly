# ifndef __ffly__chunk__h
# define __ffly__chunk__h
# include <mdlint.h>
# include "types/uni_par_t.h"
# include "types/pixelmap_t.h"
struct ffly_chunk {
    ffly_pixelmap_t pixelmap;
    ffly_uni_par_t *pars;
};


struct ffly_chunk* ffly_alloc_chunk(mdl_uint_t, mdl_uint_t, mdl_uint_t);
void ffly_free_chunk(struct ffly_chunk*);
# endif /*__ffly__chunk__h*/
