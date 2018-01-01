# include "chunk.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/alloc_pixelmap.h"
ffly_chunkp ffly_alloc_chunk(mdl_uint_t __xal, mdl_uint_t __yal, mdl_uint_t __zal) {
    ffly_chunkp chunk = (ffly_chunkp)__ffly_mem_alloc(sizeof(struct ffly_chunk));
//    chunk->pars = (ffly_uni_par_t*)__ffly_mem_alloc((__zal*(__yal*__xal))*sizeof(ffly_uni_par_t));
    chunk->pixelmap = ffly_alloc_pixelmap(__xal, __yal, __zal, 4);
    return chunk; 
}

void ffly_free_chunk(ffly_chunkp __chunk) {
//    __ffly_mem_free(__chunk->pars);
    __ffly_mem_free(__chunk->pixelmap);
    __ffly_mem_free(__chunk);
}
