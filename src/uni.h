# ifndef __ffly__uni__h
# define __ffly__uni__h
# include <mdlint.h>
# include "chunk_manager.h"
# include "types/err_t.h"
# include "types/off_t.h"
# include "types/bool_t.h"
# define ffly_uni_chunk_xal(__uni) \
    (1<<__uni->splice)
# define ffly_uni_chunk_yal(__uni) \
    (1<<__uni->splice)
# define ffly_uni_chunk_zal(__uni) \
    (1<<__uni->splice)
struct ffly_uni {
    mdl_uint_t splice;
    mdl_uint_t xcnk_c, ycnk_c, zcnk_c;
    struct ffly_cnk_man chunk_man;
    ffly_id_t *chunks;
    mdl_uint_t xal, yal, zal;
    mdl_uint_t chunk_c;
};

typedef struct ffly_uni* ffly_unip;
ffly_bool_t ffly_uni_pixcopy(ffly_unip, ffly_byte_t*, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_uint_t);
ffly_id_t ffly_uni_chunk(ffly_unip, ffly_off_t, ffly_off_t, ffly_off_t);
ffly_pixelmap_t ffly_uni_chunk_pixelmap(ffly_unip, ffly_id_t);
ffly_err_t ffly_uni_build(ffly_unip, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_u8_t);
ffly_err_t ffly_uni_free(ffly_unip);
# ifdef __cplusplus
namespace mdl {
namespace firefly {
class uni {
    types::err_t build(uint_t __xal, uint_t __yal, uint_t __zal, u8_t __splice) {
        return ffly_uni_build(&this->raw, __xal, __yal, __zal, __splice);
    }

    types::err_t free() {
        ffly_uni_free(&this->raw);
    }
    struct ffly_uni raw;
};
}
}
# endif
# endif /*__ffly__uni__h*/
