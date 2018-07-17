# include "../db.h"
# include "../dep/mem_dup.h"
# include "../memory/mem_alloc.h"
/*
	TODO:
		load from custom user file
*/
ff_db_userp
ff_db_get_user(ff_dbdp __d, ff_u8_t const *__id, ff_uint_t __il, ff_err_t *__err) {
	return ffly_map_get(&__d->users, __id, __il, __err); 
}

ff_db_userp
ff_db_add_user(ff_dbdp __d, ff_u8_t const *__id, ff_uint_t __il, ff_u32_t __passkey) {
	ff_db_userp user = (ff_db_userp)__ffly_mem_alloc(sizeof(struct ff_db_user));
	ffly_mem_dupe((void**)&user->id, __id, __il);
	user->passkey = __passkey;
	user->loggedin = -1;

	ffly_map_put(&__d->users, __id, __il, user);
	return user;
}

void
ff_db_del_user(ff_dbdp __d, ff_u8_t const *__id, ff_uint_t __il, ff_u32_t __passkey) {
	
}
