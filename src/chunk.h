# ifndef __ffly__chunk__h
# define __ffly__chunk__h
# include <mdlint.h>
# include "types/uni_par_t.h"
# include "types/pixelmap_t.h"
# include "system/vec.h"
struct ffly_chunk {
    struct ffly_vec objs;
    ffly_pixelmap_t pixelmap;
    ffly_uni_par_t *pars;
};

typedef struct ffly_chunk* ffly_chunkp;

ffly_chunkp ffly_alloc_chunk(mdl_uint_t, mdl_uint_t, mdl_uint_t);
void ffly_free_chunk(ffly_chunkp);
# endif /*__ffly__chunk__h*/
