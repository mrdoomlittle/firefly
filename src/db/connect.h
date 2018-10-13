# ifndef __ffly__db__connect__h
# define __ffly__db__connect__h
# include "../ffint.h"
# include "../db.h"
# include "../types.h"
// connector
typedef struct ff_db_ctr {
	FF_SOCKET *sock;
	ff_db_key key;
	ff_u64_t enckey;
} *ff_db_ctrp;

// connection
typedef struct ff_db_conn {
	struct ff_db_ctr ctr;

} *ff_db_connp;

# ifndef __only_struc
ff_db_ctrp ff_db_ctr(ff_u64_t, char const*, ff_u16_t, ff_err_t*); 
ff_err_t ff_db_ctr_destroy(ff_db_ctrp);
void ff_db_ctr_done(void);
ff_err_t ff_db_ctr_shutdown(ff_db_ctrp);
ff_err_t ff_db_ctr_disconnect(ff_db_ctrp);
ff_err_t ff_db_ctr_login(ff_db_ctrp, char const*, ff_u32_t);
ff_err_t ff_db_ctr_logout(ff_db_ctrp);
ff_err_t ff_db_ctr_pile_creat(ff_db_ctrp, ff_u32_t*);
ff_err_t ff_db_ctr_pile_del(ff_db_ctrp, ff_u32_t);
ff_err_t ff_db_ctr_record_creat(ff_db_ctrp, ff_u32_t, ff_u32_t*, ff_u32_t);
ff_err_t ff_db_ctr_record_del(ff_db_ctrp, ff_u32_t, ff_u32_t);
ff_err_t ff_db_ctr_write(ff_db_ctrp, ff_u32_t, ff_u32_t, ff_u32_t, void*, ff_u32_t);
ff_err_t ff_db_ctr_read(ff_db_ctrp, ff_u32_t, ff_u32_t, ff_u32_t, void*, ff_u32_t);
ff_err_t ff_db_ctr_record_alloc(ff_db_ctrp, ff_u32_t);
ff_err_t ff_db_ctr_record_free(ff_db_ctrp, ff_u32_t);
ff_err_t ff_db_ctr_rivet(ff_db_ctrp, ff_u16_t, ff_u32_t);
ff_err_t ff_db_ctr_derivet(ff_db_ctrp, ff_u16_t);
ff_err_t ff_db_ctr_rivetto(ff_db_ctrp, ff_u16_t, ff_u32_t);
ff_err_t ff_db_ctr_bind(ff_db_ctrp, ff_u16_t, ff_u32_t, ff_u8_t);
ff_err_t ff_db_ctr_acquire_slot(ff_db_ctrp, ff_u32_t*);
ff_err_t ff_db_ctr_scrap_slot(ff_db_ctrp, ff_u32_t);
ff_i8_t ff_db_ctr_exist(ff_db_ctrp, ff_u16_t, ff_err_t*);
ff_err_t ff_db_ctr_record_stat(ff_db_ctrp, ff_u32_t, ffdb_recstatp);
# endif
# endif /*__ffly__db__connect__h*/
