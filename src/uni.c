# include "uni.h"
# include "system/io.h"
# include "system/errno.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "data/mem_set.h"
# define is_sliceable(__no, __by) \
    !(__no-((__no>>__by)*(1<<__by)))

mdl_uint_t static get_cnk_off(ffly_unip __uni, mdl_uint_t __cnk_no) {
    return __cnk_no*(1<<__uni->splice);
}

mdl_uint_t static get_cnk_no(ffly_unip __uni, mdl_uint_t __off) {
    return __off>>__uni->splice;
}

ffly_chunkp static get_chunk(ffly_unip __uni, mdl_uint_t __xa, mdl_uint_t __ya, mdl_uint_t __za) {
    return ffly_cnk_man_fetch(&__uni->chunk_man, ffly_uni_chunk(__uni, __xa, __ya, __za));
}

ffly_lotpp static get_lot(ffly_unip __uni, mdl_uint_t __xa, mdl_uint_t __ya, mdl_uint_t __za) {
    ffly_chunkp chunk = get_chunk(__uni, __xa, __ya, __za);
    return ffly_fetch_lot(chunk, __xa, __ya, __za);
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
    ffly_chunkp chunk;
    while(za < __za+__zal) {
        ya = __ya;
        while(ya < __ya+__yal) {
            xa = __xa;
            while(xa < __xa+__xal) {
                chunk = get_chunk(__uni, xa, ya, za);
                ffly_lotp lot = *ffly_fetch_lot(chunk, xa, ya, za);
                if (lot != NULL) {
                    ffly_objpp itr = lot->top;
                    ffly_objp obj;
                    while(itr != lot->end) {
                        obj = *itr;
                        if ((obj->xa < __xa+__xal && obj->xa >= __xa) && (obj->ya < __ya+__yal && obj->ya >= __ya) && (obj->za < __za+__zal && obj->za >= __za)) {
                            ffly_obj_draw(obj, __dst, obj->xa-__xa, obj->ya-__ya, obj->za-__za, __xal, __yal, __zal, __xa+__xal, __ya+__yal, __za+__zal);
                        }
                        itr++;
                    }
                }
                xa+= 1<<chunk->lotsize;
            }
            ya+= 1<<chunk->lotsize;
        }
        za+= 1<<chunk->lotsize;
    }
}

ffly_id_t ffly_uni_chunk(ffly_unip __uni, ffly_off_t __xa, ffly_off_t __ya, ffly_off_t __za) {
    return *(__uni->chunks+(__xa>>__uni->splice)+((__ya>>__uni->splice)*__uni->xcnk_c)+((__za>>__uni->splice)*(__uni->ycnk_c*__uni->xcnk_c)));
}

ffly_err_t ffly_uni_free(ffly_unip __uni) {
    ffly_cnk_man_de_init(&__uni->chunk_man);
    __ffly_mem_free(__uni->chunks);
}

ffly_err_t ffly_uni_attach_obj(ffly_unip __uni, ffly_objp __obj) {
    ffly_lotpp lot = get_lot(__uni, __obj->xa, __obj->ya, __obj->za);
    if (!*lot) {
        ffly_fprintf(ffly_log, "new lot.\n");
        ffly_chunkp chunk = get_chunk(__uni, __obj->xa, __obj->ya, __obj->za);
        *lot = ffly_alloc_lot(1<<chunk->lotsize, 1<<chunk->lotsize, 1<<chunk->lotsize);
        ffly_lot_prepare(*lot, (__obj->xa>>chunk->lotsize)*chunk->lotsize, (__obj->ya>>chunk->lotsize)*chunk->lotsize, (__obj->za>>chunk->lotsize)*chunk->lotsize);
    }

//    ffly_fprintf(ffly_log, "added to lot.\n");
    ffly_lot_add(*lot, __obj);
}
    
ffly_err_t ffly_uni_detach_obj(ffly_unip __uni, ffly_objp __obj) {
    ffly_lot_rm(*get_lot(__uni, __obj->xa, __obj->ya, __obj->za), __obj);
}

ffly_err_t ffly_uni_build(ffly_unip __uni, mdl_uint_t __xal, mdl_uint_t __yal, mdl_uint_t __zal, mdl_u8_t __splice, mdl_u8_t __lotsize) {
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
    mdl_uint_t size = __uni->xcnk_c*__uni->ycnk_c*__uni->zcnk_c;
    __uni->chunks = (ffly_id_t*)__ffly_mem_alloc(size*sizeof(ffly_id_t));
    __uni->chunk_c = 0;
    mdl_uint_t xcnk, ycnk, zcnk = 0;
    while(zcnk != __uni->zcnk_c) {
        ycnk = 0;
        while(ycnk != __uni->ycnk_c) {
            xcnk = 0;
            while(xcnk != __uni->xcnk_c) {
                ffly_id_t id;
                ffly_cnk_man_create(&__uni->chunk_man, &id, xcnk*(1<<__splice), ycnk*(1<<__splice), zcnk*(1<<__splice), __lotsize);
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
