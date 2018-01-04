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

ffly_chunkp static get_chunk(ffly_unip __uni, mdl_uint_t __xa, mdl_uint_t __ya, mdl_uint_t __za) {
    return ffly_cnk_man_fetch(&__uni->chunk_man, ffly_uni_chunk(__uni, __xa, __ya, __za));
}

ffly_objpp static get_obj(ffly_unip __uni, mdl_uint_t __xa, mdl_uint_t __ya, mdl_uint_t __za) {
    ffly_chunkp chunk = get_chunk(__uni, __xa, __ya, __za);
    mdl_uint_t x = __xa-(get_cnk_off(__uni, __xa)*ffly_uni_chunk_xal(__uni));
    mdl_uint_t y = __ya-(get_cnk_off(__uni, __ya)*ffly_uni_chunk_yal(__uni));
    mdl_uint_t z = __za-(get_cnk_off(__uni, __za)*ffly_uni_chunk_zal(__uni));
    return chunk->objs+x+(y*chunk->xal)*(z*(chunk->yal*chunk->xal));
}

ffly_err_t ffly_uni_obj_move(ffly_unip __uni, ffly_objp __obj, mdl_uint_t __xa, mdl_uint_t __ya, mdl_uint_t __za) {
    ffly_uni_detach_obj(__uni, __obj);
    __obj->xa = __xa;
    __obj->ya = __ya;
    __obj->za = __za;
    ffly_uni_attach_obj(__uni, __obj);
}

ffly_bool_t ffly_uni_frame(ffly_unip __uni, ffly_byte_t *__dst, mdl_uint_t __xal, mdl_uint_t __yal, mdl_uint_t __zal, mdl_uint_t __xa, mdl_uint_t __ya, mdl_uint_t __za) {
    mdl_uint_t cnk_xal = ffly_uni_chunk_xal(__uni);
    mdl_uint_t cnk_yal = ffly_uni_chunk_yal(__uni);
    mdl_uint_t cnk_zal = ffly_uni_chunk_zal(__uni);

    mdl_uint_t xa = __xa, ya = __ya, za = __za;
    while(za != __za+__zal) {
        ya = __ya;
        while(ya != __ya+__yal) {
            xa = __xa;
            while(xa != __xa+__xal) {
                ffly_objp obj = *get_obj(__uni, xa, ya, za);
                if (obj != NULL)
                    ffly_obj_draw(obj, __dst, xa, ya, za, __xal, __yal, __zal, __xa+__xal, __ya+__yal, __za+__zal); 
                xa++;
            }
            ya++;
        }
        za++;
    }
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
//    __ffly_mem_free(__uni->objs);
}

ffly_err_t ffly_uni_attach_obj(ffly_unip __uni, ffly_objp __obj) {
    ffly_objpp obj = get_obj(__uni, __obj->xa, __obj->ya, __obj->za);
    if (!*obj)
        *obj = __obj;
}

ffly_err_t ffly_uni_detach_obj(ffly_unip __uni, ffly_objp __obj) {
    ffly_objpp obj = get_obj(__uni, __obj->xa, __obj->ya, __obj->za);
    if (*obj == __obj)
        *obj = NULL;
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
    mdl_uint_t size = __uni->zcnk_c*(__uni->ycnk_c*__uni->xcnk_c);
    __uni->chunks = (ffly_id_t*)__ffly_mem_alloc(size*sizeof(ffly_id_t));
//    __uni->objs = (ffly_objp*)__ffly_mem_alloc(size*sizeof(ffly_objpp));
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
