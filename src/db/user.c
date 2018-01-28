# include "../db.h"
# include "../data/mem_dupe.h"
# include "../memory/mem_alloc.h"
ff_db_userp ff_db_get_user(ff_dbdp __d, mdl_u8_t const *__id, mdl_uint_t __il, ffly_err_t *__err) {
    return ffly_map_get(&__d->users, __id, __il, __err); 
}

ff_db_userp ff_db_add_user(ff_dbdp __d, mdl_u8_t const *__id, mdl_uint_t __il, mdl_u32_t __passkey) {
    ff_db_userp user = (ff_db_userp)__ffly_mem_alloc(sizeof(struct ff_db_user));
    ffly_mem_dupe((void**)&user->id, __id, __il);
    user->passkey = __passkey;
    user->loggedin = 0;

    ffly_map_put(&__d->users, __id, __il, user);
    return user;
}

void ff_db_del_user(ff_dbdp __d, mdl_u8_t const *__id, mdl_uint_t __il, mdl_u32_t __passkey) {
    
}
