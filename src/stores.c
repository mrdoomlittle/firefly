# include "stores.h"
# include "system/config.h"
# include "db/connect.h"
# include "system/util/hash.h"
# include "dep/str_len.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# define FF_STORES_MODT 1
# define FF_STORES_INFO 0
ff_db_ctrp static ct;
void ff_stores_connect() {
	ff_err_t err;
	char const *enckey = __ffly_sysconf__.db.enckey;
	ct = ff_db_ctr(ffly_hash(enckey, ffly_str_len(enckey)), __ffly_sysconf__.db.ip_addr, __ffly_sysconf__.db.port, &err);	
}

void ff_stores_login() {
	char const *passwd = __ffly_sysconf__.db.passwd;
	ff_db_ctr_login(ct, __ffly_sysconf__.db.user, ffly_hash(passwd, ffly_str_len(passwd)));
}

void static*
motd() {
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
}

void* ff_stores_get(ff_u8_t __id) {
	switch(__id) {
		case _ff_stores_motd: return motd();		
	}
}

void ff_stores_logout() {
	ff_db_ctr_logout(ct);
}

void ff_stores_disconnect() {
	ff_db_ctr_disconnect(ct);
	ff_db_ctr_destroy(ct);
}
