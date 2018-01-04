# include "chunk.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
ffly_chunkp ffly_alloc_chunk(mdl_uint_t __xal, mdl_uint_t __yal, mdl_uint_t __zal) {
    ffly_chunkp chunk = (ffly_chunkp)__ffly_mem_alloc(sizeof(struct ffly_chunk));
    chunk->objs = (ffly_objpp)__ffly_mem_alloc((__zal*(__yal*__xal))*sizeof(ffly_objp));
    mdl_uint_t xa, ya, za = 0;
    while(za != __zal) {
        ya = 0;
        while(ya != __yal) {
            xa = 0;
            while(xa != __xal) {
                *(chunk->objs+xa+(ya*__xal)+(za*(__yal*__xal))) = NULL;
                xa++;
            }
            ya++;
        }
        za++;
    }
    chunk->xal = __xal;
    chunk->yal = __yal;
    chunk->zal = __zal;
    return chunk; 
}

void ffly_free_chunk(ffly_chunkp __chunk) {
//    __ffly_mem_free(__chunk->pars);
    __ffly_mem_free(__chunk->objs);
    __ffly_mem_free(__chunk);
}
