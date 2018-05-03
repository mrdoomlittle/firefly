# ifndef __ffly__uni__h
# define __ffly__uni__h
# include "ffint.h"
# include "types.h"
# include "lot.h"
# include "physics/body.h"
# include "chunk_manager.h"
# define ffly_uni_chunk_xal(__uni) \
    (1<<__uni->splice)
# define ffly_uni_chunk_yal(__uni) \
    (1<<__uni->splice)
# define ffly_uni_chunk_zal(__uni) \
    (1<<__uni->splice)

enum {
    _ffly_uni_8 = 8,
    _ffly_uni_16 = 16,
    _ffly_uni_32 = 32,
    _ffly_uni_64 = 64,
    _ffly_uni_128 = 128,
    _ffly_uni_256 = 256,
    _ffly_uni_512 = 512,
    _ffly_uni_1024 = 1024,
    _ffly_uni_2048 = 2048,
    _ffly_uni_4096 = 4096
};

enum {
    _ffly_splice_8 = 3,
    _ffly_splice_16,
    _ffly_splice_32,
    _ffly_splice_64,
    _ffly_splice_128,
};

typedef struct ffly_uni {
    ff_uint_t splice;
    ff_uint_t xcnk_c, ycnk_c, zcnk_c;
    struct ffly_cnk_man chunk_man;
    ff_id_t *chunks;
    ff_uint_t xl, yl, zl;
    ff_uint_t chunk_c;
} *ffly_unip;

# ifdef __cplusplus
extern "C" {
# endif
void ffly_uni_update(ffly_unip, ff_uint_t);
ffly_lotp ffly_uni_get_lot(ffly_unip, ff_uint_t, ff_uint_t, ff_uint_t);
ff_err_t ffly_uni_attach_body(ffly_unip, ffly_phy_bodyp);
ff_err_t ffly_uni_detach_body(ffly_unip, ffly_phy_bodyp);
ff_err_t ffly_uni_body_move(ffly_unip, ffly_phy_bodyp, ff_uint_t, ff_uint_t, ff_uint_t);
/* capture frame */
ff_err_t ffly_uni_frame(ffly_unip, ff_byte_t*, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t);
ff_id_t ffly_uni_chunk(ffly_unip, ff_off_t, ff_off_t, ff_off_t);
ff_err_t ffly_uni_build(ffly_unip, ff_uint_t, ff_uint_t, ff_uint_t, ff_u8_t, ff_u8_t);
ff_err_t ffly_uni_free(ffly_unip);
# ifdef __cplusplus
}
/*
# include "obj.h"
namespace mdl {
namespace firefly {
struct uni {
    types::err_t build(uint_t __xl, uint_t __yl, uint_t __zl, u8_t __splice, u8_t __lotsize) {
        return ffly_uni_build(&this->raw, __xl, __yl, __zl, __splice, __lotsize);
    }
    types::err_t attach_obj(obj& __obj) {
        return ffly_uni_attach_obj(&this->raw, __obj.raw);
    }
    types::err_t detach_obj(obj& __obj) {
        return ffly_uni_detach_obj(&this->raw, __obj.raw);
    }
    types::err_t move_obj(obj __obj, uint_t __x, uint_t __y, uint_t __z) {
        return ffly_uni_obj_move(&this->raw, __obj.raw, __x, __y, __z);
    }
    types::err_t move_obj(obj *__obj, uint_t __x, uint_t __y, uint_t __z) {
        return ffly_uni_obj_move(&this->raw, __obj->raw, __x, __y, __z);
    }
    types::err_t move_obj(obj& __obj, uint_t __x, uint_t __y, uint_t __z) {
        return ffly_uni_obj_move(&this->raw, __obj.raw, __x, __y, __z);
    }
    types::err_t frame(types::byte_t *__frame, uint_t __xl, uint_t __yl, uint_t __zl, uint_t __x, uint_t __y, uint_t __z) {
        return ffly_uni_frame(&this->raw, __frame, __xl, __yl, __zl, __x, __y, __z);
    }
    types::err_t free() {
        ffly_uni_free(&this->raw);
    }
    struct ffly_uni raw;
};
}
}
*/
# endif
# endif /*__ffly__uni__h*/
