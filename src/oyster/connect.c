# include "../oyster.h"
# include "../system/io.h"
# include "../system/err.h"
# include "../dep/str_cmp.h"
# include "../system/string.h"
# include "../dep/bzero.h"
# include "../inet.h"
# include "../system/nanosleep.h"
# include "connect.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../dep/mem_cpy.h"
# include "../dep/str_len.h"
# include "../crypto.h"
#ifdef __ffly_debug
# include "../location.h"
#endif

ff_u16_t static stack = 0;
ff_db_err static get_errno(FF_SOCKET*, ff_err_t*);

ff_err_t static ff_db_shutdown(FF_SOCKET*);
ff_err_t static ff_db_login(FF_SOCKET*, ff_u16_t, ff_u16_t, ff_u32_t, ff_u8_t*, ff_u64_t);
ff_err_t static ff_db_logout(FF_SOCKET*, ff_u8_t*, ff_u64_t);
ff_err_t static ff_db_store(FF_SOCKET*, void*, ff_u16_t, ff_u16_t);
ff_err_t static ff_db_load(FF_SOCKET*, void*, ff_u16_t, ff_u16_t);
ff_err_t static ff_db_disconnect(FF_SOCKET*);
ff_err_t static ff_db_pile_creat(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_u32_t*);
ff_err_t static ff_db_pile_del(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_u32_t);
ff_err_t static ff_db_record_creat(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_u32_t, ff_u32_t*, ff_u32_t);
ff_err_t static ff_db_record_del(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_u32_t, ff_u32_t);
ff_err_t static ff_db_write(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_u32_t, ff_u32_t, ff_u32_t, void*, ff_u32_t);
ff_err_t static ff_db_read(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_u32_t, ff_u32_t, ff_u32_t, void*, ff_u32_t);
ff_err_t static ff_db_record_alloc(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_u32_t);
ff_err_t static ff_db_record_free(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_u32_t);
ff_err_t static ff_db_rivet(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_u16_t, ff_u32_t);
ff_err_t static ff_db_derivet(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_u16_t);
ff_err_t static ff_db_rivetto(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_u16_t, ff_u32_t);
ff_err_t static ff_db_bind(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_u16_t, ff_u32_t, ff_u8_t);
ff_err_t static ff_db_acquire_slot(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_u32_t*);
ff_err_t static ff_db_scrap_slot(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_u32_t);
ff_i8_t static ff_db_exist(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_u16_t, ff_err_t*);
ff_err_t static ff_db_record_stat(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_u32_t, ffdb_recstatp);

/*
	TODO:
		get auth table and notify user that they dont have access
		flag will have to be set to override
*/

ff_db_ctrp
ff_db_ctr(ff_u64_t __enckey, char const *__ip_adr, ff_u16_t __port, ff_err_t *__err) {
	_location_push(_ff_loc_db_ctr);
	ff_db_ctrp ret = (ff_db_ctrp)__ffly_mem_alloc(sizeof(struct ff_db_ctr));
	ret->enckey = __enckey;
	ff_err_t err;
	struct sockaddr_in adr;
	ret->sock = ff_net_creat(&err, _NET_PROT_TCP);
	if (_err(err)) {
		*__err = err;
		goto _fail;
	}

	ffly_bzero(&adr, sizeof(struct sockaddr_in));
	adr.sin_addr.s_addr = inet_addr(__ip_adr);
	adr.sin_family = AF_INET;
	adr.sin_port = htons(__port);
	if (_err(err = ff_net_connect(ret->sock, (struct sockaddr*)&adr, sizeof(struct sockaddr_in)))) {
		*__err = err;
		ff_net_close(ret->sock);
		goto _fail;
	}

	*__err = FFLY_SUCCESS;
	goto _succ;
_fail:
	__ffly_mem_free(ret);
	ret = NULL;
_succ:
	_location_pop();
	return ret;
}

void
ff_db_ctr_done(void) {
	stack = 0;
}

ff_err_t ff_db_ctr_destroy(ff_db_ctrp __ctr) {
	ffly_sock_shutdown(__ctr->sock, SHUT_RDWR);
	ff_net_close(__ctr->sock);
	__ffly_mem_free(__ctr);
}

ff_err_t
ff_db_ctr_shutdown(ff_db_ctrp __ctr) {
	return ff_db_shutdown(__ctr->sock);
}

ff_err_t
ff_db_ctr_disconnect(ff_db_ctrp __ctr) {
	return ff_db_disconnect(__ctr->sock);
}


ff_err_t
ff_db_ctr_login(ff_db_ctrp __ctr, char const *__id, ff_u32_t __passkey) {
	_location_push(_ff_loc_db_ctr_login);
	ff_err_t err;
	if (!__ctr) {
		err = FFLY_FAILURE;
		goto _fail;
	}

	ff_uint_t id_len;
	id_len = ffly_str_len(__id);
	ff_u16_t id;
	id = stack;
	ff_db_store(__ctr->sock, __id, id, id_len);
	stack+=id_len;
	err = ff_db_login(__ctr->sock, id, id_len, __passkey, __ctr->key, __ctr->enckey);
_fail:
	_location_pop();
	return err;
}

ff_err_t
ff_db_ctr_logout(ff_db_ctrp __ctr) {
	return ff_db_logout(__ctr->sock, __ctr->key, __ctr->enckey);
}

ff_err_t
ff_db_ctr_pile_creat(ff_db_ctrp __ctr, ff_u32_t *__slot) {
	return ff_db_pile_creat(__ctr->sock, __ctr->key, __ctr->enckey, __slot);
}

ff_err_t
ff_db_ctr_pile_del(ff_db_ctrp __ctr, ff_u32_t __slot) {
	return ff_db_pile_del(__ctr->sock, __ctr->key, __ctr->enckey, __slot);
}

ff_err_t
ff_db_ctr_record_creat(ff_db_ctrp __ctr, ff_u32_t __pile, ff_u32_t *__slot, ff_u32_t __size) {
	return ff_db_record_creat(__ctr->sock, __ctr->key, __ctr->enckey, __pile, __slot, __size);
}

ff_err_t
ff_db_ctr_record_del(ff_db_ctrp __ctr, ff_u32_t __pile, ff_u32_t __slot) {
	return ff_db_record_del(__ctr->sock, __ctr->key, __ctr->enckey, __pile, __slot);
}

ff_err_t
ff_db_ctr_write(ff_db_ctrp __ctr, ff_u32_t __pile, ff_u32_t __rec, ff_u32_t __offset, void *__buf, ff_u32_t __size) {
	return ff_db_write(__ctr->sock, __ctr->key, __ctr->enckey, __pile, __rec, __offset, __buf, __size);
}

ff_err_t
ff_db_ctr_read(ff_db_ctrp __ctr, ff_u32_t __pile, ff_u32_t __rec, ff_u32_t __offset, void *__buf, ff_u32_t __size) {
	return ff_db_read(__ctr->sock, __ctr->key, __ctr->enckey, __pile, __rec, __offset, __buf, __size);
}

ff_err_t
ff_db_ctr_record_alloc(ff_db_ctrp __ctr, ff_u32_t __rec) {
	return ff_db_record_alloc(__ctr->sock, __ctr->key, __ctr->enckey, __rec);
}

ff_err_t
ff_db_ctr_record_free(ff_db_ctrp __ctr, ff_u32_t __rec) {
	return ff_db_record_free(__ctr->sock, __ctr->key, __ctr->enckey, __rec);
}

ff_err_t
ff_db_ctr_rivet(ff_db_ctrp __ctr, ff_u16_t __rivet, ff_u32_t __slot) {
	return ff_db_rivet(__ctr->sock, __ctr->key, __ctr->enckey, __rivet, __slot);
}

ff_err_t
ff_db_ctr_derivet(ff_db_ctrp __ctr, ff_u16_t __rivet) {
	return ff_db_derivet(__ctr->sock, __ctr->key, __ctr->enckey, __rivet);
}

ff_err_t
ff_db_ctr_rivetto(ff_db_ctrp __ctr, ff_u16_t __rivet, ff_u32_t __slot) {
	return ff_db_rivetto(__ctr->sock, __ctr->key, __ctr->enckey, __rivet, __slot);
}

ff_err_t
ff_db_ctr_bind(ff_db_ctrp __ctr, ff_u16_t __rivet, ff_u32_t __slot, ff_u8_t __offset) {
	return ff_db_bind(__ctr->sock, __ctr->key, __ctr->enckey, __rivet, __slot, __offset);
}

ff_err_t
ff_db_ctr_acquire_slot(ff_db_ctrp __ctr, ff_u32_t *__slot) {
	return ff_db_acquire_slot(__ctr->sock, __ctr->key, __ctr->enckey, __slot);
}

ff_err_t
ff_db_ctr_scrap_slot(ff_db_ctrp __ctr, ff_u32_t __slot) {
	return ff_db_scrap_slot(__ctr->sock, __ctr->key, __ctr->enckey, __slot);
}

ff_i8_t
ff_db_ctr_exist(ff_db_ctrp __ctr, ff_u16_t __rivet, ff_err_t *__err) {
	return ff_db_exist(__ctr->sock, __ctr->key, __ctr->enckey, __rivet, __err);
}

ff_err_t
ff_db_ctr_record_stat(ff_db_ctrp __ctr, ff_u32_t __rec, ffdb_recstatp __st) {
	return ff_db_record_stat(__ctr->sock, __ctr->key, __ctr->enckey, __rec, __st);
}

# define tape(__sock, __code, __len, __err)   \
{   \
    ff_u32_t len;   \
    len = __len;    \
    ff_net_send(__sock, &len, sizeof(ff_u32_t), 0, __err);  \
    ff_net_send(__sock, __code, __len, 0, __err);   \
}

ff_i8_t static
ratifykey(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey) {
	ff_u8_t code[1+_FF_RATIFYKEY_S];
	*code = _ff_db_op_ratifykey;
	ffly_mem_cpy(code+1, __key, KEY_SIZE);
	ffly_encrypt(code+1, KEY_SIZE, __enckey);

	ff_err_t err;
	tape(__sock, code, 1+_FF_RATIFYKEY_S, &err);
	ff_i8_t res;
	ff_net_recv(__sock, &res, 1, 0, &err);
	return res;
}

ff_err_t
ff_db_login(FF_SOCKET *__sock, ff_u16_t __id, ff_u16_t __il,
    ff_u32_t __passkey, ff_u8_t *__key, ff_u64_t __enckey)
{
	ff_u8_t code[1+_FF_LOGIN_S];
	*code = _ff_db_op_login;
	*(ff_u16_t*)(code+1) = __id;
	*(ff_u16_t*)(code+3) = __il;
	*(ff_u32_t*)(code+5) = __passkey;
	ff_err_t err;
	tape(__sock, code, 1+_FF_LOGIN_S, &err);

	ff_u8_t key[KEY_SIZE];
	ff_net_recv(__sock, key, KEY_SIZE, 0, &err);
	ffly_decrypt(key, KEY_SIZE, __enckey);
	ffly_mem_cpy(__key, key, KEY_SIZE);
	retok;
}

ff_err_t
ff_db_logout(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey) {
	if (ratifykey(__sock, __key, __enckey) == -1) {

	}
	ff_err_t err;
	ff_u8_t code;
	code = _ff_db_op_logout;
	tape(__sock, &code, 1, &err);
	retok;
}

ff_err_t
ff_db_store(FF_SOCKET *__sock, void *__src, ff_u16_t __addr, ff_u16_t __n) {
	ff_err_t err;
	ff_u8_t code[5];
	*code = _ff_db_op_store;
	*(ff_u16_t*)(code+1) = __addr;
	*(ff_u16_t*)(code+3) = __n;
	tape(__sock, code, 5, &err);
	ff_net_send(__sock, __src, __n, 0, &err);
	retok;
}

ff_err_t
ff_db_load(FF_SOCKET *__sock, void *__dst, ff_u16_t __addr, ff_u16_t __n) {
	ff_err_t err;
	ff_u8_t code[5];
	*code = _ff_db_op_load;
	*(ff_u16_t*)(code+1) = __addr;
	*(ff_u16_t*)(code+3) = __n;
	tape(__sock, code, 5, &err);
	ff_net_recv(__sock, __dst, __n, 0, &err);
	retok;
}

ff_err_t
ff_db_shutdown(FF_SOCKET *__sock) {
	ff_err_t err;
	ff_u8_t code;
	code = _ff_db_op_shutdown;
	tape(__sock, &code, 1, &err);
	retok;
}

ff_err_t 
ff_db_pile_creat(FF_SOCKET *__sock, ff_u8_t *__key,
	ff_u64_t __enckey, ff_u32_t *__slot)
{
	ff_err_t err;
	if (ratifykey(__sock, __key, __enckey) == -1) {

	}

	ff_u8_t code[1+_FF_PILE_CREAT_S];
	*code = _ff_db_op_pile_creat;
	tape(__sock, code, 1+_FF_PILE_CREAT_S, &err);

	ff_net_recv(__sock, __slot, sizeof(ff_u32_t), 0, &err);
	retok;
}

ff_err_t
ff_db_pile_del(FF_SOCKET *__sock, ff_u8_t *__key,
	ff_u64_t __enckey, ff_u32_t __slot)
{
	ff_err_t err;
	if (ratifykey(__sock, __key, __enckey) == -1) {

	}

	ff_u8_t code[1+_FF_PILE_DEL_S];
	*code = _ff_db_op_pile_del;
	*(ff_u32_t*)(code+1) = __slot;
	tape(__sock, code, 1+_FF_PILE_DEL_S, &err);
}

ff_err_t 
ff_db_record_creat(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey,
	ff_u32_t __pile, ff_u32_t *__slot, ff_u32_t __size)
{
	ff_err_t err;
	if (ratifykey(__sock, __key, __enckey) == -1) {

	}

	ff_u8_t code[1+_FF_RECORD_CREAT_S];
	*code = _ff_db_op_record_creat;
	
	*(ff_u32_t*)(code+1) = __pile;
	*(ff_u32_t*)(code+5) = __size;
	tape(__sock, code, 1+_FF_RECORD_CREAT_S, &err);
	ff_net_recv(__sock, __slot, sizeof(ff_u32_t), 0, &err);
}

ff_err_t
ff_db_record_del(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey,
	ff_u32_t __pile, ff_u32_t __slot)
{
	ff_err_t err;
	if (ratifykey(__sock, __key, __enckey) == -1) {

	}

	ff_u8_t code[1+_FF_RECORD_DEL_S];
	*code = _ff_db_op_record_del;
	*(ff_u32_t*)(code+1) = __pile;
	*(ff_u32_t*)(code+5) = __slot;
	tape(__sock, code, 1+_FF_RECORD_DEL_S, &err);
}

ff_err_t
ff_db_write(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey, ff_u32_t __pile,
	ff_u32_t __rec, ff_u32_t __offset, void *__buf, ff_u32_t __size)
{
	ff_err_t err;
	if (ratifykey(__sock, __key, __enckey) == -1) {

	}

	ff_u8_t code[1+_FF_WRITE_S];
	*code = _ff_db_op_write;
	*(ff_u32_t*)(code+1) = __pile;
	*(ff_u32_t*)(code+5) = __rec;
	*(ff_u32_t*)(code+9) = __offset;
	*(ff_u32_t*)(code+13) = __size;
	tape(__sock, code, 1+_FF_WRITE_S, &err);
	ff_net_send(__sock, __buf, __size, 0, &err);	
}

ff_err_t
ff_db_read(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey, ff_u32_t __pile,
	ff_u32_t __rec, ff_u32_t __offset, void *__buf, ff_uint_t __size)
{
	ff_err_t err;
	if (ratifykey(__sock, __key, __enckey) == -1) {

	}

	ff_u8_t code[1+_FF_READ_S];
	*code = _ff_db_op_read;
	*(ff_u32_t*)(code+1) = __pile;
	*(ff_u32_t*)(code+5) = __rec;
	*(ff_u32_t*)(code+9) = __offset;
	*(ff_u32_t*)(code+13) = __size;
	tape(__sock, code, 1+_FF_READ_S, &err);
	ff_net_recv(__sock, __buf, __size, 0, &err);	
}

ff_err_t
ff_db_record_alloc(FF_SOCKET *__sock, ff_u8_t *__key,
	ff_u64_t __enckey, ff_u32_t __rec)
{
	ff_err_t err;
	if (ratifykey(__sock, __key, __enckey) == -1) {

	}

	ff_u8_t code[1+_FF_RECORD_ALLOC_S];
	*code = _ff_db_op_record_alloc;
	*(ff_u32_t*)(code+1) = __rec;
	tape(__sock, code, 1+_FF_RECORD_ALLOC_S, &err);
}

ff_err_t
ff_db_record_free(FF_SOCKET *__sock, ff_u8_t *__key,
	ff_u64_t __enckey, ff_u32_t __rec)
{
	ff_err_t err;
	if (ratifykey(__sock, __key, __enckey) == -1) {

	}

	ff_u8_t code[1+_FF_RECORD_FREE_S];
	*code = _ff_db_op_record_free;
	*(ff_u32_t*)(code+1) = __rec;
	tape(__sock, code, 1+_FF_RECORD_FREE_S, &err);
}

ff_err_t
ff_db_rivet(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey,
	ff_u16_t __rivet, ff_u32_t __slot)
{
	ff_err_t err;
	if (ratifykey(__sock, __key, __enckey) == -1) {

	}

	ff_u8_t code[1+_FF_RIVET_S];
	*code = _ff_db_op_rivet;
	*(ff_u32_t*)(code+1) = __slot;
	*(ff_u16_t*)(code+5) = __rivet;
	tape(__sock, code, 1+_FF_RIVET_S, &err);
}

ff_err_t
ff_db_derivet(FF_SOCKET *__sock, ff_u8_t *__key,
	ff_u64_t __enckey, ff_u16_t __rivet)
{
	ff_err_t err;
	if (ratifykey(__sock, __key, __enckey) == -1) {

	}

	ff_u8_t code[1+_FF_DERIVET_S];
	*code = _ff_db_op_derivet;
	*(ff_u16_t*)(code+1) = __rivet;
	tape(__sock, code, 1+_FF_DERIVET_S, &err);
}

ff_err_t
ff_db_rivetto(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey,
	ff_u16_t __rivet, ff_u32_t __slot)
{
	ff_err_t err;
	if (ratifykey(__sock, __key, __enckey) == -1) {

	}

	ff_u8_t code[1+_FF_RIVETTO_S];
	*code = _ff_db_op_rivetto;
	*(ff_u32_t*)(code+1) = __slot;
	*(ff_u16_t*)(code+5) = __rivet;
	tape(__sock, code, 1+_FF_RIVETTO_S, &err);
}

ff_err_t
ff_db_bind(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey,
	ff_u16_t __rivet, ff_u32_t __slot, ff_u8_t __offset)
{
	ff_err_t err;
	if (ratifykey(__sock, __key, __enckey) == -1) {

	}

	ff_u8_t code[1+_FF_BIND_S];
	*code = _ff_db_op_bind;
	*(ff_u32_t*)(code+1) = __slot;
	*(ff_u16_t*)(code+5) = __rivet;
	*(code+7) = __offset;
	tape(__sock, code, 1+_FF_BIND_S, &err);
}

ff_err_t
ff_db_acquire_slot(FF_SOCKET *__sock, ff_u8_t *__key,
	ff_u64_t __enckey, ff_u32_t *__slot)
{
	ff_err_t err;
	if (ratifykey(__sock, __key, __enckey) == -1) {

	}

	ff_u8_t code[1+_FF_ACQUIRE_SLOT_S];
	*code = _ff_db_op_acquire_slot;
	tape(__sock, code, 1+_FF_ACQUIRE_SLOT_S, &err);
	ff_net_recv(__sock, __slot, sizeof(ff_u32_t), 0, &err);
}

ff_err_t
ff_db_scrap_slot(FF_SOCKET *__sock, ff_u8_t *__key,
	ff_u64_t __enckey, ff_u32_t __slot)
{
	ff_err_t err;
	if (ratifykey(__sock, __key, __enckey) == -1) {

	}

	ff_u8_t code[1+_FF_SCRAP_SLOT_S];
	*code = _ff_db_op_scrap_slot;
	*(ff_u32_t*)(code+1) = __slot;
	tape(__sock, code, 1+_FF_SCRAP_SLOT_S, &err);
}

ff_i8_t
ff_db_exist(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey,
	ff_u16_t __rivet, ff_err_t *__err)
{
	ff_err_t err;
	if (ratifykey(__sock, __key, __enckey) == -1) {

	}

	ff_u8_t code[1+_FF_EXIST_S];
	*code = _ff_db_op_exist;
	*(ff_u16_t*)(code+1) = __rivet;
	tape(__sock, code, 1+_FF_EXIST_S, &err);

	ff_i8_t r;
	ff_net_recv(__sock, &r, sizeof(ff_i8_t), 0, &err);
	return r;
}

ff_err_t
ff_db_record_stat(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey,
	ff_u32_t __rec, ffdb_recstatp __st)
{
	ff_err_t err;
	if (ratifykey(__sock, __key, __enckey) == -1) {

	}

	ff_u8_t code[1+_FF_RECORD_STAT_S];
	*code = _ff_db_op_recstat;
	*(ff_u32_t*)(code+1) = __rec;
	tape(__sock, code, 1+_FF_RECORD_STAT_S, &err);
	ff_net_recv(__sock, __st, sizeof(struct ffdb_recstat), 0, &err);
}

ff_err_t ff_db_disconnect(FF_SOCKET *__sock) {
	ff_err_t err;
	ff_u8_t code[1];
	*code = _ff_db_op_disconnect;
	tape(__sock, code, 1, &err);
}
