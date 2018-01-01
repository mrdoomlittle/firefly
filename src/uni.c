# include "uni.h"
# include "system/io.h"
# include "system/errno.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "data/mem_set.h"
# define is_sliceable(__no, __by) \
    (!(__no-((__no>>__by)*(1<<__by))))
mdl_uint_t static get_cnk_off(ffly_unip __uni, mdl_uint_t __off) {
    return __off>>__uni->splice;
}

ffly_bool_t ffly_uni_pixcopy(ffly_unip __uni, ffly_byte_t *__dst, mdl_uint_t __xal, mdl_uint_t __yal, mdl_uint_t __zal, mdl_uint_t __xa, mdl_uint_t __ya, mdl_uint_t __za) {
    mdl_uint_t cnk_xal = ffly_uni_chunk_xal(__uni);
    mdl_uint_t cnk_yal = ffly_uni_chunk_yal(__uni);
    mdl_uint_t cnk_zal = ffly_uni_chunk_zal(__uni);

    mdl_uint_t xa = __xa, ya = __ya, za = __za;
    while(za != __za+__zal) {
        ya = __ya;
        while(ya != __ya+__yal) {
            xa = __xa;
            while(xa != __xa+__xal) {
                ffly_byte_t *chunk = ffly_uni_chunk_pixelmap(__uni, ffly_uni_chunk(__uni, xa, ya, za));
                mdl_uint_t x = xa-(get_cnk_off(__uni, xa)*cnk_xal);
                mdl_uint_t y = ya-(get_cnk_off(__uni, ya)*cnk_yal);
                mdl_uint_t z = za-(get_cnk_off(__uni, za)*cnk_zal);
                ffly_byte_t *src = chunk+((x+(y*cnk_xal)+(z*(cnk_xal*cnk_yal)))*4);
                ffly_byte_t *dst = __dst+(((xa-__xa)+((ya-__ya)*__xal)+((za-__za)*(__xal*__yal)))*4);
                dst[0] = src[0];
                dst[1] = src[1];
                dst[2] = src[2];
                dst[3] = src[3];
                xa++;
            }
            ya++;
        }
        za++;
    }
}

ffly_pixelmap_t ffly_uni_chunk_pixelmap(ffly_unip __uni, ffly_id_t __id) {
    return ffly_cnk_man_fetch(&__uni->chunk_man, __id)->pixelmap;
}

ffly_id_t ffly_uni_chunk(ffly_unip __uni, ffly_off_t __xa, ffly_off_t __ya, ffly_off_t __za) {
    return *(__uni->chunks+(__xa>>__uni->splice)+((__ya>>__uni->splice)*__uni->xcnk_c)+((__za>>__uni->splice)*(__uni->ycnk_c*__uni->xcnk_c)));
}

ffly_err_t ffly_uni_free(ffly_unip __uni) {
    ffly_cnk_man_de_init(&__uni->chunk_man);
    ffly_id_t *itr = __uni->chunks;
    while(itr != __uni->chunks+__uni->chunk_c) {
        __ffly_mem_free(*itr);
        itr++;
    }

    __ffly_mem_free(__uni->chunks);
}

ffly_err_t ffly_uni_build(ffly_unip __uni, mdl_uint_t __xal, mdl_uint_t __yal, mdl_uint_t __zal, mdl_u8_t __splice) {
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
                if (*id == 0)
                    ffly_mem_set(ffly_cnk_man_fetch(&__uni->chunk_man, id)->pixelmap, 100, (1<<__splice)*(1<<__splice)*(1<<__splice)*4);
                else if (*id == 1) 
                    ffly_mem_set(ffly_cnk_man_fetch(&__uni->chunk_man, id)->pixelmap, 200, (1<<__splice)*(1<<__splice)*(1<<__splice)*4);
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
