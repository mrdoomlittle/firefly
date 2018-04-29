# ifndef __ffly__chunk__manager__h
# define __ffly__chunk__manager__h
# include "ffint.h"
# include "types.h"
# include "chunk.h"
# include "system/barrel.h"

struct ffly_cnk_man {
    ff_uint_t xal, yal, zal; 
    struct ffly_chunk **chunks;

	ffly_barrelp barrel;
    ff_off_t off;
    ff_uint_t no_chunks;
};
# ifdef __cplusplus
extern "C" {
# endif
ff_err_t ffly_cnk_man_init(struct ffly_cnk_man*, ff_uint_t, ff_uint_t, ff_uint_t);
ff_err_t ffly_cnk_man_de_init(struct ffly_cnk_man*);
ff_err_t ffly_cnk_man_create(struct ffly_cnk_man*, ff_id_t*, ff_uint_t, ff_uint_t, ff_uint_t, ff_u8_t);
struct ffly_chunk* ffly_cnk_man_fetch(struct ffly_cnk_man*, ff_id_t);
ff_err_t ffly_cnk_man_del(struct ffly_cnk_man*, ff_id_t);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__chunk__manager__h*/
