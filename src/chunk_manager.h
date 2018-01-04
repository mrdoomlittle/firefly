# ifndef __ffly__chunk__manager__h
# define __ffly__chunk__manager__h
# include "types/id_t.h"
# include "types/err_t.h"
# include "chunk.h"
# include "types/off_t.h"
struct ffly_cnk_man {
    mdl_uint_t xal, yal, zal; 
    struct ffly_chunk **chunks;
    ffly_off_t off;
    mdl_uint_t no_chunks;
};

ffly_err_t ffly_cnk_man_init(struct ffly_cnk_man*, mdl_uint_t, mdl_uint_t, mdl_uint_t);
ffly_err_t ffly_cnk_man_de_init(struct ffly_cnk_man*);
ffly_err_t ffly_cnk_man_create(struct ffly_cnk_man*, ffly_id_t*, mdl_uint_t, mdl_uint_t, mdl_uint_t, mdl_u8_t);
struct ffly_chunk* ffly_cnk_man_fetch(struct ffly_cnk_man*, ffly_id_t);
ffly_err_t ffly_cnk_man_del(struct ffly_cnk_man*, ffly_id_t);
# endif /*__ffly__chunk__manager__h*/
