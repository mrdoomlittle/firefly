# include "../db.h"
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
# include "../dep/str_len.h"
# ifdef __ffly_debug
# include "../location.h"
# endif
ff_db_err static get_errno(FF_SOCKET*, ff_err_t*);

ff_err_t static ff_db_shutdown(FF_SOCKET*);
ff_err_t static ff_db_disconnect(FF_SOCKET*);
ff_err_t static ff_db_login(FF_SOCKET*, ff_u8_t const*, ff_uint_t, ff_u32_t, ff_u8_t*, ff_u64_t);
ff_err_t static ff_db_logout(FF_SOCKET*, ff_u8_t*, ff_u64_t);
ff_err_t static ff_db_creat_pile(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_uint_t*);
ff_err_t static ff_db_del_pile(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_uint_t);
ff_err_t static ff_db_creat_record(FF_SOCKET*, ff_u8_t*, ff_u64_t,ff_uint_t, ff_uint_t*, ff_uint_t);
ff_err_t static ff_db_del_record(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_uint_t, ff_uint_t);
ff_err_t static ff_db_write(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_uint_t, ff_uint_t, ff_u32_t, void*, ff_uint_t);
ff_err_t static ff_db_read(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_uint_t, ff_uint_t, ff_u32_t, void*, ff_uint_t);
ff_err_t static ff_db_record_alloc(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_uint_t);
ff_err_t static ff_db_record_free(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_uint_t);
ff_err_t static ff_db_rivet(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_u16_t, ff_uint_t);
ff_err_t static ff_db_derivet(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_u16_t);
ff_err_t static ff_db_rivetto(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_u16_t, ff_uint_t);
ff_err_t static ff_db_bind(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_u16_t, ff_uint_t, ff_u8_t);
ff_err_t static ff_db_acquire_slot(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_uint_t*);
ff_err_t static ff_db_scrap_slot(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_uint_t);
ff_i8_t static ff_db_exist(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_u16_t, ff_err_t*);
ff_err_t static ff_db_record_stat(FF_SOCKET*, ff_u8_t*, ff_u64_t, ff_uint_t, ffdb_recstatp);

ff_db_ctrp
ff_db_ctr(ff_u64_t __enckey, char const *__ip_adr, ff_u16_t __port, ff_err_t *__err) {
# ifdef __ffly_debug
	ff_location_push(_ff_loc_db_ctr);
# endif
	ff_db_ctrp ret = (ff_db_ctrp)__ffly_mem_alloc(sizeof(struct ff_db_ctr));
	ret->enckey = __enckey;
	ff_err_t err;
	struct sockaddr_in adr;
	ret->sock = ff_net_creat(&err, AF_INET, SOCK_STREAM, 0);
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
# ifdef __ffly_debug
	ff_location_pop();
# endif
	return ret;
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
# ifdef __ffly_debug
	ff_location_push(_ff_loc_db_ctr_login);
# endif
	ff_err_t err;
	if (!__ctr) {
		err = FFLY_FAILURE;
		goto _fail;
	}
	err = ff_db_login(__ctr->sock, (ff_u8_t const*)__id, ffly_str_len(__id), __passkey, __ctr->key, __ctr->enckey);
_fail:
# ifdef __ffly_debug
	ff_location_pop();
# endif
	return err;
}

ff_err_t
ff_db_ctr_logout(ff_db_ctrp __ctr) {
	return ff_db_logout(__ctr->sock, __ctr->key, __ctr->enckey);
}

ff_err_t
ff_db_ctr_creat_pile(ff_db_ctrp __ctr, ff_uint_t *__slotno) {
	return ff_db_creat_pile(__ctr->sock, __ctr->key, __ctr->enckey, __slotno);
}

ff_err_t
ff_db_ctr_del_pile(ff_db_ctrp __ctr, ff_uint_t __slotno) {
	return ff_db_del_pile(__ctr->sock, __ctr->key, __ctr->enckey, __slotno);
}

ff_err_t
ff_db_ctr_creat_record(ff_db_ctrp __ctr, ff_uint_t __pile, ff_uint_t *__slotno, ff_uint_t __size) {
	return ff_db_creat_record(__ctr->sock, __ctr->key, __ctr->enckey, __pile, __slotno, __size);
}

ff_err_t
ff_db_ctr_del_record(ff_db_ctrp __ctr, ff_uint_t __pile, ff_uint_t __slotno) {
	return ff_db_del_record(__ctr->sock, __ctr->key, __ctr->enckey, __pile, __slotno);
}

ff_err_t
ff_db_ctr_write(ff_db_ctrp __ctr, ff_uint_t __pile, ff_uint_t __rec, ff_u32_t __offset, void *__buf, ff_uint_t __size) {
	return ff_db_write(__ctr->sock, __ctr->key, __ctr->enckey, __pile, __rec, __offset, __buf, __size);
}

ff_err_t
ff_db_ctr_read(ff_db_ctrp __ctr, ff_uint_t __pile, ff_uint_t __rec, ff_u32_t __offset, void *__buf, ff_uint_t __size) {
	return ff_db_read(__ctr->sock, __ctr->key, __ctr->enckey, __pile, __rec, __offset, __buf, __size);
}

ff_err_t
ff_db_ctr_record_alloc(ff_db_ctrp __ctr, ff_uint_t __rec) {
	return ff_db_record_alloc(__ctr->sock, __ctr->key, __ctr->enckey, __rec);
}

ff_err_t
ff_db_ctr_record_free(ff_db_ctrp __ctr, ff_uint_t __rec) {
	return ff_db_record_free(__ctr->sock, __ctr->key, __ctr->enckey, __rec);
}

ff_err_t
ff_db_ctr_rivet(ff_db_ctrp __ctr, ff_u16_t __rivetno, ff_uint_t __slotno) {
	return ff_db_rivet(__ctr->sock, __ctr->key, __ctr->enckey, __rivetno, __slotno);
}

ff_err_t
ff_db_ctr_derivet(ff_db_ctrp __ctr, ff_u16_t __rivetno) {
	return ff_db_derivet(__ctr->sock, __ctr->key, __ctr->enckey, __rivetno);
}

ff_err_t
ff_db_ctr_rivetto(ff_db_ctrp __ctr, ff_u16_t __rivetno, ff_uint_t __slotno) {
	return ff_db_rivetto(__ctr->sock, __ctr->key, __ctr->enckey, __rivetno, __slotno);
}

ff_err_t
ff_db_ctr_bind(ff_db_ctrp __ctr, ff_u16_t __rivetno, ff_uint_t __slotno, ff_u8_t __offset) {
	return ff_db_bind(__ctr->sock, __ctr->key, __ctr->enckey, __rivetno, __slotno, __offset);
}

ff_err_t
ff_db_ctr_acquire_slot(ff_db_ctrp __ctr, ff_uint_t *__slotno) {
	return ff_db_acquire_slot(__ctr->sock, __ctr->key, __ctr->enckey, __slotno);
}

ff_err_t
ff_db_ctr_scrap_slot(ff_db_ctrp __ctr, ff_uint_t __slotno) {
	return ff_db_scrap_slot(__ctr->sock, __ctr->key, __ctr->enckey, __slotno);
}

ff_i8_t
ff_db_ctr_exist(ff_db_ctrp __ctr, ff_u16_t __rivetno, ff_err_t *__err) {
	return ff_db_exist(__ctr->sock, __ctr->key, __ctr->enckey, __rivetno, __err);
}

ff_err_t
ff_db_ctr_record_stat(ff_db_ctrp __ctr, ff_uint_t __rec, ffdb_recstatp __st) {
	return ff_db_record_stat(__ctr->sock, __ctr->key, __ctr->enckey, __rec, __st);
}

ff_i8_t static
ratifykey(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey) {
	ff_err_t err;
	ff_err_t fault;
	if (_err(err = ff_db_snd_key(__sock, __key, __enckey))) {
		return -1;
	}
	ff_db_rcv_err(__sock, &fault);
	if (_err(fault)) {
		ff_db_err ern = get_errno(__sock, &err);
		ffly_printf("errstr: %s\n", ff_db_errst(ern));
		return -1;
	}
	return 0;
}

ff_err_t
ff_db_shutdown(FF_SOCKET *__sock) {
	ffly_printf("shutdown.\n");
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_shutdown
	};

	ff_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {

	}

	ff_db_rcv_err(__sock, &err);
	if (_err(err)) {
		ffly_printf("errstr: %s\n", ff_db_errst(get_errno(__sock, &err)));
		reterr;
	}
	retok;
}

ff_err_t
ff_db_disconnect(FF_SOCKET *__sock) {
	ffly_printf("disconnect.\n");
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_disconnect
	};

	ff_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {

	}

	ff_db_rcv_err(__sock, &err);
	if (_err(err)) {
		ffly_printf("errstr: %s\n", ff_db_errst(get_errno(__sock, &err)));
		reterr;
	}
	retok;
}

ff_db_err static
get_errno(FF_SOCKET *__sock, ff_err_t *__err) {
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_req_errno
	};

	ff_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {

	}

	ff_db_rcv_err(__sock, &err);
	if (_err(err)) {

	}

	ff_db_err ern;
	ff_db_rcv_errno(__sock, &ern);
	return ern;
}

ff_err_t
ff_db_login(FF_SOCKET *__sock, ff_u8_t const *__id, ff_uint_t __il,
	ff_u32_t __passkey, ff_u8_t *__key, ff_u64_t __enckey)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_login
	};

	ff_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {
	
	}

	ff_db_rcv_err(__sock, &err);
	if (_err(err)) {
		ffly_printf("errstr: %s\n", ff_db_errst(get_errno(__sock, &err)));
		return err;
	}

	ffly_printf("sent message.\n");
	ff_net_send(__sock, &__il, sizeof(ff_uint_t), &err);
	ffly_printf("sent id length.\n");

	ff_net_send(__sock, __id, __il, &err);
	ffly_printf("sent id.\n");
	ff_net_send(__sock, &__passkey, sizeof(ff_u32_t), &err);
	ffly_printf("sent passkey.\n");

	ff_db_rcv_err(__sock, &err);
	if (_err(err)) {
		ffly_printf("errstr: %s\n", ff_db_errst(get_errno(__sock, &err)));
		return err;
	}

	ff_db_rcv_key(__sock, __key, __enckey);
	retok;
}

ff_err_t
ff_db_logout(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey) {
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_logout
	};

	ff_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {
		ffly_printf("failed to send message.\n");
	} 

	ff_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {
		ffly_printf("failed to recv error.\n");
	}

	if (_err(fault)) {
		ffly_printf("errstr: %s\n", ff_db_errst(get_errno(__sock, &err)));
		return err;
	}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}
	retok;
}

ff_err_t
ff_db_creat_pile(FF_SOCKET *__sock, ff_u8_t *__key,
	ff_u64_t __enckey, ff_uint_t *__slotno)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_creat_pile
	};

	ff_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {
		ffly_printf("failed to send message.\n");
	}

	ff_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {
		ffly_printf("failed to recv error.\n");
	}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_recv(__sock, __slotno, sizeof(ff_uint_t), &err);
	if (_err(err)) {
		ffly_printf("failed to recv slotno.\n");
	}
	retok;
}

ff_err_t
ff_db_del_pile(FF_SOCKET *__sock, ff_u8_t *__key,
	ff_u64_t __enckey, ff_uint_t __slotno)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_del_pile
	};

	ff_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {
		ffly_printf("failed to send message.\n");
	}

	ff_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {
		ffly_printf("failed to recv error.\n");
	}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__slotno, sizeof(ff_uint_t), &err);
	if (_err(err)) {
		ffly_printf("failed to send slotno.\n");
	}
	retok;
}

ff_err_t
ff_db_creat_record(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey,
	ff_uint_t __pile, ff_uint_t *__slotno, ff_uint_t __size)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_creat_record
	};

	ff_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {
		ffly_printf("failed to send message.\n");
	}

	ff_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {
	}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__pile, sizeof(ff_uint_t), &err);
	ff_net_send(__sock, &__size, sizeof(ff_uint_t), &err);
	ff_net_recv(__sock, __slotno, sizeof(ff_uint_t), &err);
	retok;
}

ff_err_t
ff_db_del_record(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey,
	ff_uint_t __pile, ff_uint_t __slotno)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_del_record
	};

	ff_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {
		ffly_printf("failed to send message.\n");
	}

	ff_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {
	}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__pile, sizeof(ff_uint_t), &err);
	ff_net_send(__sock, &__slotno, sizeof(ff_uint_t), &err);
	retok;
}

ff_err_t
ff_db_write(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey, ff_uint_t __pile,
	ff_uint_t __rec, ff_u32_t __offset, void *__buf, ff_uint_t __size)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_write
	};

	ff_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ff_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__pile, sizeof(ff_uint_t), &err);
	ff_net_send(__sock, &__rec, sizeof(ff_uint_t), &err);
	ff_net_send(__sock, &__offset, sizeof(ff_u32_t), &err);
	ff_net_send(__sock, &__size, sizeof(ff_uint_t), &err);
	ff_net_send(__sock, __buf, __size, &err);
	retok;
}

ff_err_t
ff_db_read(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey, ff_uint_t __pile,
	ff_uint_t __rec, ff_u32_t __offset, void *__buf, ff_uint_t __size)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_read
	};

	ff_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ff_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__pile, sizeof(ff_uint_t), &err);
	ff_net_send(__sock, &__rec, sizeof(ff_uint_t), &err);
	ff_net_send(__sock, &__offset, sizeof(ff_u32_t), &err);
	ff_net_send(__sock, &__size, sizeof(ff_uint_t), &err);
	ff_net_recv(__sock, __buf, __size, &err);
	retok;
}

ff_err_t
ff_db_record_alloc(FF_SOCKET *__sock, ff_u8_t *__key,
	ff_u64_t __enckey, ff_uint_t __rec)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_record_alloc
	};

	ff_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ff_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__rec, sizeof(ff_uint_t), &err);
	retok;
}

ff_err_t
ff_db_record_free(FF_SOCKET *__sock, ff_u8_t *__key,
	ff_u64_t __enckey, ff_uint_t __rec)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_record_free
	};

	ff_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ff_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__rec, sizeof(ff_uint_t), &err);
	retok;
}

ff_err_t
ff_db_rivet(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey,
	ff_u16_t __rivetno, ff_uint_t __slotno)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_rivet
	};

	ff_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ff_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__slotno, sizeof(ff_uint_t), &err);
	ff_net_send(__sock, &__rivetno, sizeof(ff_u16_t), &err);
	retok;
}

ff_err_t
ff_db_derivet(FF_SOCKET *__sock, ff_u8_t *__key,
	ff_u64_t __enckey, ff_u16_t __rivetno)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_derivet
	};

	ff_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ff_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__rivetno, sizeof(ff_u16_t), &err);
	retok;
}

ff_err_t
ff_db_rivetto(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey,
	ff_u16_t __rivetno, ff_uint_t __slotno)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_rivetto
	};

	ff_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ff_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__slotno, sizeof(ff_uint_t), &err);
	ff_net_send(__sock, &__rivetno, sizeof(ff_u16_t), &err);
	retok;
}

ff_err_t
ff_db_bind(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey,
	ff_u16_t __rivetno, ff_uint_t __slotno, ff_u8_t __offset)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_bind
	};

	ff_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ff_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__slotno, sizeof(ff_uint_t), &err);
	ff_net_send(__sock, &__rivetno, sizeof(ff_u16_t), &err);
	ff_net_send(__sock, &__offset, sizeof(ff_u8_t), &err);
	retok;
}

ff_err_t 
ff_db_acquire_slot(FF_SOCKET *__sock, ff_u8_t *__key,
	ff_u64_t __enckey, ff_uint_t *__slotno)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_acquire_slot
	};

	ff_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ff_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_recv(__sock, __slotno, sizeof(ff_uint_t), &err);
	retok;
}

ff_err_t
ff_db_scrap_slot(FF_SOCKET *__sock, ff_u8_t *__key,
	ff_u64_t __enckey, ff_uint_t __slotno)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_scrap_slot
	};

	ff_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ff_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__slotno, sizeof(ff_uint_t), &err);
	retok;
}

ff_i8_t 
ff_db_exist(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey,
	ff_u16_t __rivetno, ff_err_t *__err)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_exist
	};

	ff_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ff_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		*__err = FFLY_FAILURE;
		return -1;
	}

	ff_net_send(__sock, &__rivetno, sizeof(ff_u16_t), &err);

	ff_i8_t ret;
	ff_net_recv(__sock, &ret, sizeof(ff_u8_t), &err);
	return ret;
}

ff_err_t
ff_db_record_stat(FF_SOCKET *__sock, ff_u8_t *__key, ff_u64_t __enckey,
	ff_uint_t __rec, ffdb_recstatp __st)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_recstat
	};

	ff_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ff_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__rec, sizeof(ff_uint_t), &err);
	ff_net_recv(__sock, __st, sizeof(struct ffdb_recstat), &err);
}
