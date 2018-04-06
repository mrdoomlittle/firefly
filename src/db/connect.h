# ifndef __ffly__db__connect__h
# define __ffly__db__connect__h
# include <mdlint.h>
# include "../db.h"
# include "../types/err_t.h"
typedef struct ff_db_ctr {
	FF_SOCKET *sock;
	ffdb_key key;
	mdl_u64_t enckey;
} *ff_db_ctrp;

ff_db_ctrp ff_db_ctr(mdl_u64_t, char const*, mdl_u16_t, ffly_err_t*); 
void ff_db_ctr_destroy(ff_db_ctrp);

ffly_err_t ff_db_ctr_shutdown(ff_db_ctrp);
ffly_err_t ff_db_ctr_login(ff_db_ctrp, char const*, mdl_u32_t);
ffly_err_t ff_db_ctr_logout(ff_db_ctrp);
ffly_err_t ff_db_ctr_creat_pile(ff_db_ctrp, mdl_uint_t*);
ffly_err_t ff_db_ctr_del_pile(ff_db_ctrp, mdl_uint_t);
ffly_err_t ff_db_ctr_creat_record(ff_db_ctrp, mdl_uint_t, mdl_uint_t*, mdl_uint_t);
ffly_err_t ff_db_ctr_del_record(ff_db_ctrp, mdl_uint_t, mdl_uint_t);
ffly_err_t ff_db_ctr_write(ff_db_ctrp, mdl_uint_t, mdl_uint_t, mdl_u32_t, void*, mdl_uint_t);
ffly_err_t ff_db_ctr_read(ff_db_ctrp, mdl_uint_t, mdl_uint_t, mdl_u32_t, void*, mdl_uint_t);
ffly_err_t ff_db_ctr_record_alloc(ff_db_ctrp, mdl_uint_t);
ffly_err_t ff_db_ctr_record_free(ff_db_ctrp, mdl_uint_t);
ffly_err_t ff_db_ctr_rivet(ff_db_ctrp, mdl_u16_t, mdl_uint_t);
ffly_err_t ff_db_ctr_derivet(ff_db_ctrp, mdl_u16_t);
ffly_err_t ff_db_ctr_rivetto(ff_db_ctrp, mdl_u16_t, mdl_uint_t);
ffly_err_t ff_db_ctr_bind(ff_db_ctrp, mdl_u16_t, mdl_uint_t, mdl_u8_t);
ffly_err_t ff_db_ctr_acquire_slot(ff_db_ctrp, mdl_uint_t*);
ffly_err_t ff_db_ctr_scrap_slot(ff_db_ctrp, mdl_uint_t);
mdl_i8_t ff_db_ctr_exist(ff_db_ctrp, mdl_u16_t, ffly_err_t*);

# endif /*__ffly__db__connect__h*/
