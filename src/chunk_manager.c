# include "chunk_manager.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"
# include "system/errno.h"
# include "system/io.h"
ff_err_t ffly_cnk_man_init(struct ffly_cnk_man *__cnk_man, ff_uint_t __xal, ff_uint_t __yal, ff_uint_t __zal) {
    __cnk_man->xal = __xal;
    __cnk_man->yal = __yal;
    __cnk_man->zal = __zal;
    __cnk_man->chunks = NULL;
    __cnk_man->off = 0;
    __cnk_man->no_chunks = 0;
	__cnk_man->barrel = NULL;
}

ff_err_t ffly_cnk_man_de_init(struct ffly_cnk_man *__cnk_man) {
	ffly_barrelp barrel = __cnk_man->barrel, bk;
	while(barrel != NULL) {
		bk = barrel;
		barrel = barrel->link;
		ffly_barrel_de_init(bk);
		__ffly_mem_free(bk);
	}

    if (__cnk_man->chunks != NULL)
        __ffly_mem_free(__cnk_man->chunks);
}

ff_err_t ffly_cnk_man_create(struct ffly_cnk_man *__cnk_man, ff_id_t *__id, ff_uint_t __xa, ff_uint_t __ya, ff_uint_t __za, ff_u8_t __lotsize) {
    ffly_fprintf(ffly_log, "creating chunk, %u at %ux%ux%u\n", __cnk_man->no_chunks, __xa, __ya, __za);
    if (!__cnk_man->chunks) {
        __cnk_man->chunks = (struct ffly_chunk**)__ffly_mem_alloc(sizeof(struct ffly_chunk*));
        __cnk_man->no_chunks++;
    } else
        __cnk_man->chunks = (struct ffly_chunk**)__ffly_mem_realloc(__cnk_man->chunks, (++__cnk_man->no_chunks)*sizeof(struct ffly_chunk*));

    struct ffly_chunk *chunk = (*(__cnk_man->chunks+__cnk_man->off) = ffly_chunk_alloc(__cnk_man->xal, __cnk_man->yal, __cnk_man->zal, __lotsize));
    if (!chunk) {
        ffly_fprintf(ffly_err, "chunk failed to allocate.\n");
    }
    ffly_chunk_prepare(chunk, __xa, __ya, __za);

	if (!__cnk_man->barrel) {
		__cnk_man->barrel = (ffly_barrelp)__ffly_mem_alloc(sizeof(struct ffly_barrel));
		ffly_barrel_init(__cnk_man->barrel, sizeof(__ff_id_t));
	}

	ffly_barrelp barrel = __cnk_man->barrel;
_bk:
	if (ffly_barrel_full(barrel)) {
		if (barrel->link != NULL) {
			barrel = barrel->link;
			goto _bk;
		}

		ffly_barrelp rep = (ffly_barrelp)__ffly_mem_alloc(sizeof(struct ffly_barrel));
		ffly_barrel_init(rep, sizeof(__ff_id_t));
		ffly_barrel_link(rep, __cnk_man->barrel);
		__cnk_man->barrel = rep;
		barrel = __cnk_man->barrel;
	}

	*__id = (ff_id_t)ffly_barrel_alloc(barrel);
	**__id = __cnk_man->off++;
    return FFLY_SUCCESS;
}

struct ffly_chunk* ffly_cnk_man_fetch(struct ffly_cnk_man *__cnk_man, ff_id_t __id) {
    return *(__cnk_man->chunks+(*__id));
}

ff_err_t ffly_cnk_man_del(struct ffly_cnk_man *__cnk_man, ff_id_t __id) {

}
