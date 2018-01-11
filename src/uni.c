# include "uni.h"
# include "system/errno.h"
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

ffly_chunkp static get_chunk(ffly_unip __uni, mdl_uint_t __x, mdl_uint_t __y, mdl_uint_t __z) {
    return ffly_cnk_man_fetch(&__uni->chunk_man, ffly_uni_chunk(__uni, __x, __y, __z));
}

ffly_lotpp static get_lot(ffly_unip __uni, mdl_uint_t __x, mdl_uint_t __y, mdl_uint_t __z) {
    ffly_chunkp chunk = get_chunk(__uni, __x, __y, __z);
    return ffly_fetch_lot(chunk, __x, __y, __z);
}

ffly_err_t ffly_uni_obj_move(ffly_unip __uni, ffly_objp __obj, mdl_uint_t __x, mdl_uint_t __y, mdl_uint_t __z) {
    ffly_uni_detach_obj(__uni, __obj);
    __obj->x = __x;
    __obj->y = __y;
    __obj->z = __z;
    ffly_uni_attach_obj(__uni, __obj);
}

ffly_err_t ffly_uni_frame(ffly_unip __uni, ffly_byte_t *__dst, mdl_uint_t __xl, mdl_uint_t __yl, mdl_uint_t __zl, mdl_uint_t __x, mdl_uint_t __y, mdl_uint_t __z) {
    mdl_uint_t cnk_xl = ffly_uni_chunk_xal(__uni);
    mdl_uint_t cnk_yl = ffly_uni_chunk_yal(__uni);
    mdl_uint_t cnk_zl = ffly_uni_chunk_zal(__uni);

    mdl_uint_t x = __x, y = __y, z = __z;
    ffly_chunkp chunk;
    while(z < __z+__zl) {
        y = __y;
        while(y < __y+__yl) {
            x = __x;
            while(x < __x+__xl) {
                chunk = get_chunk(__uni, x, y, z);
                ffly_lotp lot = *ffly_fetch_lot(chunk, x, y, z);
                if (lot != NULL) {
                    ffly_objpp itr = lot->top;
                    ffly_objp obj;
                    while(itr != lot->end) {
                        obj = *itr;
                        if ((obj->x < __x+__xl && obj->x >= __x) && (obj->y < __y+__yl && obj->y >= __y) && (obj->z < __z+__zl && obj->z >= __z)) {
                            ffly_obj_draw(obj, __dst, obj->x-__x, obj->y-__y, 0, __xl, __yl, __x+__xl, __y+__yl);
                        }
                        itr++;
                    }
                }
                x+= 1<<chunk->lotsize;
            }
            y+= 1<<chunk->lotsize;
        }
        z+= 1<<chunk->lotsize;
    }
}

ffly_id_t ffly_uni_chunk(ffly_unip __uni, ffly_off_t __x, ffly_off_t __y, ffly_off_t __z) {
    return *(__uni->chunks+(__x>>__uni->splice)+((__y>>__uni->splice)*__uni->xcnk_c)+((__z>>__uni->splice)*(__uni->ycnk_c*__uni->xcnk_c)));
}

ffly_err_t ffly_uni_free(ffly_unip __uni) {
    ffly_cnk_man_de_init(&__uni->chunk_man);
    __ffly_mem_free(__uni->chunks);
}

ffly_err_t ffly_uni_attach_obj(ffly_unip __uni, ffly_objp __obj) {
    ffly_lotpp lot = get_lot(__uni, __obj->x, __obj->y, __obj->z);
    if (!*lot) {
        ffly_fprintf(ffly_log, "new lot.\n");
        ffly_chunkp chunk = get_chunk(__uni, __obj->x, __obj->y, __obj->z);
        *lot = ffly_alloc_lot(1<<chunk->lotsize, 1<<chunk->lotsize, 1<<chunk->lotsize);
        ffly_lot_prepare(*lot, (__obj->x>>chunk->lotsize)*chunk->lotsize, (__obj->y>>chunk->lotsize)*chunk->lotsize, (__obj->z>>chunk->lotsize)*chunk->lotsize);
    }

//    ffly_fprintf(ffly_log, "added to lot.\n");
    ffly_lot_add(*lot, __obj);
    __obj->lot = *lot;
}
    
ffly_err_t ffly_uni_detach_obj(ffly_unip __uni, ffly_objp __obj) {
    if (!__uni || !__obj) {
        ffly_fprintf(ffly_err, "object detach failed.\n");
        return FFLY_FAILURE;
    }
    ffly_lot_rm(__obj->lot, __obj);
}

ffly_err_t ffly_uni_build(ffly_unip __uni, mdl_uint_t __xl, mdl_uint_t __yl, mdl_uint_t __zl, mdl_u8_t __splice, mdl_u8_t __lotsize) {
    if (!is_sliceable(__xl, __splice)) {
        ffly_fprintf(ffly_err, "length of x not sliceable by %u.\n", 1<<__splice);
        return FFLY_FAILURE;
    }

    if (!is_sliceable(__yl, __splice)) {
        ffly_fprintf(ffly_err, "length of y not sliceable by %u.\n", 1<<__splice);
        return FFLY_FAILURE;

    }

    if (!is_sliceable(__zl, __splice)) {
        ffly_fprintf(ffly_err, "length of z sliceable by %u.\n", 1<<__splice);
        return FFLY_FAILURE;
    }
    
    ffly_cnk_man_init(&__uni->chunk_man, 1<<__splice, 1<<__splice, 1<<__splice);
    __uni->xcnk_c = __xl>>__splice;
    __uni->ycnk_c = __yl>>__splice;
    __uni->zcnk_c = __zl>>__splice;
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
