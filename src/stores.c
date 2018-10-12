# include "stores.h"
# include "db/connect.h"
# include "system/util/hash.h"
# include "dep/str_len.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/error.h"
# ifdef __ffly_debug
# include "location.h"
# endif
# define FF_STORES_MODT 1
# define FF_STORES_INFO 0
ff_db_ctrp static ct = NULL;

ff_err_t ff_stores_connect(char const *__ip_addr, ff_u16_t __port, char const *__enckey) {
# ifdef __ffly_debug
	ff_location_push(_ff_loc_stores_connect);
# endif
	ff_err_t err;
	ct = ff_db_ctr(ffly_hash(__enckey, ffly_str_len(__enckey)), __ip_addr, __port, &err);	
# ifdef __ffly_debug
	if (_err(err)) {
		ffly_fprintf(ffly_err, "failed to create connector.\n");
	}

	ff_location_pop();
# endif
	return err;
}

ff_err_t ff_stores_login(char const *__user, char const *__passwd) {
/*
# ifdef __ffly_debug
	ff_location_push(_ff_loc_stores_login);
# endif
	ff_err_t err;
	if (_err(err = ff_db_ctr_login(ct, __user, ffly_hash(__passwd, ffly_str_len(__passwd))))) {
		
	}
# ifdef __ffly_debug
	ff_location_pop();
# endif
	return err;
*/
}

void static*
motd(void) {
/*
	ff_uint_t pile, rec;
	ff_db_ctr_acquire_slot(ct, &pile);
	ff_db_ctr_acquire_slot(ct, &rec);
	ff_db_ctr_rivetto(ct, FF_STORES_INFO, pile);
	ff_db_ctr_rivetto(ct, FF_STORES_MODT, rec);

	struct ffdb_recstat st;
	ff_db_ctr_record_stat(ct, rec, &st);
	char *p = (char*)__ffly_mem_alloc(st.size);
	ff_db_ctr_read(ct, pile, rec, 0, p, st.size);
	ff_db_ctr_scrap_slot(ct, pile);
	ff_db_ctr_scrap_slot(ct, rec);
	return p;
*/
}

void* ff_stores_get(ff_u8_t __id) {
	switch(__id) {
		case _ff_stores_motd: return motd();		
	}
}

ff_err_t ff_stores_logout(void) {
/*
# ifdef __ffly_debug
	ff_location_push(_ff_loc_stores_login);
# endif
	ff_err_t err;
	if (_err(err = ff_db_ctr_logout(ct))) {
# ifdef __ffly_debug
		ffly_fprintf(ffly_err, "failed to logout.\n");
# endif
	}
# ifdef __ffly_debug
	ff_location_pop();
# endif
	return err;
*/
}

ff_err_t ff_stores_disconnect(void) {
/*
# ifdef __ffly_debug
	ff_location_push(_ff_loc_stores_login);
# endif
	ff_err_t err;
	if (_err(err = ff_db_ctr_disconnect(ct))) {
# ifdef __ffly_debug
		ffly_fprintf(ffly_err, "failed to disconnect.\n");
# endif
	}
*/
	ff_err_t err;
	if (_err(err = ff_db_ctr_destroy(ct))) {
# ifdef __ffly_debug
		ffly_fprintf(ffly_err, "failed to destory.\n");
# endif
	}
# ifdef __ffly_debug
	ff_location_pop();
# endif
}
