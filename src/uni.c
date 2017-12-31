# include "uni.h"
# include "system/io.h"
# include "system/errno.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# define is_sliceable(__no, __by) \
    (!(__no-((__no>>__by)*(1<<__by))))

ffly_pixelmap_t ffly_uni_chunk_pixelmap(struct ffly_uni *__uni, ffly_id_t __id) {
    return ffly_cnk_man_fetch(&__uni->chunk_man, __id)->pixelmap;
}

ffly_id_t ffly_uni_chunk(struct ffly_uni *__uni, ffly_off_t __xa, ffly_off_t __ya, ffly_off_t __za) {
    return *(__uni->chunks+(__xa>>__uni->splice)+((__ya>>__uni->splice)*__uni->xcnk_c)+((__za>>__uni->splice)*(__uni->ycnk_c*__uni->xcnk_c)));
}

ffly_err_t ffly_uni_free(struct ffly_uni *__uni) {
    ffly_cnk_man_de_init(&__uni->chunk_man);
    ffly_id_t *itr = __uni->chunks;
    while(itr != __uni->chunks+__uni->chunk_c) {
        __ffly_mem_free(*itr);
        itr++;
    }

    __ffly_mem_free(__uni->chunks);
}

ffly_err_t ffly_uni_build(struct ffly_uni *__uni, mdl_uint_t __xal, mdl_uint_t __yal, mdl_uint_t __zal, mdl_u8_t __splice) {
    if (!is_sliceable(__xal, __splice)) {
        ffly_fprintf(ffly_err, "xal not sliceable by %u.\n", 1<<__splice);
        return FFLY_FAILURE;
    }

    if (!is_sliceable(__yal, __splice)) {
        ffly_fprintf(ffly_err, "yal not sliceable by %u.\n", 1<<__splice);
        return FFLY_FAILURE;

    }

    if (!is_sliceable(__zal, __splice)) {
        ffly_fprintf(ffly_err, "zal not sliceable by %u.\n", 1<<__splice);
        return FFLY_FAILURE;
    }
    
    ffly_cnk_man_init(&__uni->chunk_man, 1<<__splice, 1<<__splice, 1<<__splice);
    __uni->xcnk_c = __xal>>__splice;
    __uni->ycnk_c = __yal>>__splice;
    __uni->zcnk_c = __zal>>__splice;
    ffly_fprintf(ffly_log, "building univirse, xcnk %u, ycnk %u, zcnk %u\n", __uni->xcnk_c, __uni->ycnk_c, __uni->zcnk_c);
    __uni->chunks = (ffly_id_t*)__ffly_mem_alloc((__uni->zcnk_c*(__uni->ycnk_c*__uni->xcnk_c))*sizeof(ffly_id_t));

    __uni->chunk_c = 0;
    mdl_uint_t zcnk, ycnk, xcnk;
    zcnk = 0;
    while(zcnk != __uni->zcnk_c) {
        ycnk = 0;
        while(ycnk != __uni->ycnk_c) {
            xcnk = 0;
            while(xcnk != __uni->xcnk_c) {
                ffly_id_t id;
                ffly_cnk_man_create(&__uni->chunk_man, &id);
                *(__uni->chunks+xcnk+(ycnk*__uni->xcnk_c)+(zcnk*(__uni->ycnk_c*__uni->xcnk_c))) = id;
                xcnk++;
                __uni->chunk_c++;
            }
            ycnk++;
        }
        zcnk++;
    }
    __uni->splice = __splice;
    return FFLY_SUCCESS;
}
