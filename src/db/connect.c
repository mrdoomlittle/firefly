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
ff_db_err static get_errno(FF_SOCKET*, ffly_err_t*);

ffly_err_t static ff_db_shutdown(FF_SOCKET*);
ffly_err_t static ff_db_disconnect(FF_SOCKET*);
ffly_err_t static ff_db_login(FF_SOCKET*, mdl_u8_t const*, mdl_uint_t, mdl_u32_t, mdl_u8_t*, mdl_u64_t);
ffly_err_t static ff_db_logout(FF_SOCKET*, mdl_u8_t*, mdl_u64_t);
ffly_err_t static ff_db_creat_pile(FF_SOCKET*, mdl_u8_t*, mdl_u64_t, mdl_uint_t*);
ffly_err_t static ff_db_del_pile(FF_SOCKET*, mdl_u8_t*, mdl_u64_t, mdl_uint_t);
ffly_err_t static ff_db_creat_record(FF_SOCKET*, mdl_u8_t*, mdl_u64_t,mdl_uint_t, mdl_uint_t*, mdl_uint_t);
ffly_err_t static ff_db_del_record(FF_SOCKET*, mdl_u8_t*, mdl_u64_t, mdl_uint_t, mdl_uint_t);
ffly_err_t static ff_db_write(FF_SOCKET*, mdl_u8_t*, mdl_u64_t, mdl_uint_t, mdl_uint_t, mdl_u32_t, void*, mdl_uint_t);
ffly_err_t static ff_db_read(FF_SOCKET*, mdl_u8_t*, mdl_u64_t, mdl_uint_t, mdl_uint_t, mdl_u32_t, void*, mdl_uint_t);
ffly_err_t static ff_db_record_alloc(FF_SOCKET*, mdl_u8_t*, mdl_u64_t, mdl_uint_t);
ffly_err_t static ff_db_record_free(FF_SOCKET*, mdl_u8_t*, mdl_u64_t, mdl_uint_t);
ffly_err_t static ff_db_rivet(FF_SOCKET*, mdl_u8_t*, mdl_u64_t, mdl_u16_t, mdl_uint_t);
ffly_err_t static ff_db_derivet(FF_SOCKET*, mdl_u8_t*, mdl_u64_t, mdl_u16_t);
ffly_err_t static ff_db_rivetto(FF_SOCKET*, mdl_u8_t*, mdl_u64_t, mdl_u16_t, mdl_uint_t);
ffly_err_t static ff_db_bind(FF_SOCKET*, mdl_u8_t*, mdl_u64_t, mdl_u16_t, mdl_uint_t, mdl_u8_t);
ffly_err_t static ff_db_acquire_slot(FF_SOCKET*, mdl_u8_t*, mdl_u64_t, mdl_uint_t*);
ffly_err_t static ff_db_scrap_slot(FF_SOCKET*, mdl_u8_t*, mdl_u64_t, mdl_uint_t);
mdl_i8_t static ff_db_exist(FF_SOCKET*, mdl_u8_t*, mdl_u64_t, mdl_u16_t, ffly_err_t*);

ff_db_ctrp
ff_db_ctr(mdl_u64_t __enckey, char const *__ip_adr, mdl_u16_t __port, ffly_err_t *__err) {
	ff_db_ctrp ret = (ff_db_ctrp)__ffly_mem_alloc(sizeof(struct ff_db_ctr));
	ret->enckey = __enckey;
	ffly_err_t err;
	ret->sock = ff_net_creat(&err, AF_INET, SOCK_STREAM, 0);
	if (_err(err)) {
		*__err = err;
		return NULL;
	}

	struct sockaddr_in adr;
	ffly_bzero(&adr, sizeof(struct sockaddr_in));
	adr.sin_addr.s_addr = inet_addr(__ip_adr);
	adr.sin_family = AF_INET;
	adr.sin_port = htons(__port);
	if (_err(err = ff_net_connect(ret->sock, (struct sockaddr*)&adr, sizeof(struct sockaddr_in)))) {
		*__err = err;
		return NULL;
	}
	*__err = FFLY_SUCCESS;
	return ret;
}

void ff_db_ctr_destroy(ff_db_ctrp __ctr) {
	ffly_nanosleep(1, 0); // change me
	ff_net_close(__ctr->sock);
	__ffly_mem_free(__ctr);
}

ffly_err_t
ff_db_ctr_shutdown(ff_db_ctrp __ctr) {
	return ff_db_shutdown(__ctr->sock);
}

ffly_err_t
ff_db_ctr_disconnect(ff_db_ctrp __ctr) {
	return ff_db_disconnect(__ctr->sock);
}

ffly_err_t
ff_db_ctr_login(ff_db_ctrp __ctr, char const *__id, mdl_u32_t __passkey) {
	return ff_db_login(__ctr->sock, (mdl_u8_t const*)__id, ffly_str_len(__id), __passkey, __ctr->key, __ctr->enckey);
}

ffly_err_t
ff_db_ctr_logout(ff_db_ctrp __ctr) {
	return ff_db_logout(__ctr->sock, __ctr->key, __ctr->enckey);
}

ffly_err_t
ff_db_ctr_creat_pile(ff_db_ctrp __ctr, mdl_uint_t *__slotno) {
	return ff_db_creat_pile(__ctr->sock, __ctr->key, __ctr->enckey, __slotno);
}

ffly_err_t
ff_db_ctr_del_pile(ff_db_ctrp __ctr, mdl_uint_t __slotno) {
	return ff_db_del_pile(__ctr->sock, __ctr->key, __ctr->enckey, __slotno);
}

ffly_err_t
ff_db_ctr_creat_record(ff_db_ctrp __ctr, mdl_uint_t __pile, mdl_uint_t *__slotno, mdl_uint_t __size) {
	return ff_db_creat_record(__ctr->sock, __ctr->key, __ctr->enckey, __pile, __slotno, __size);
}

ffly_err_t
ff_db_ctr_del_record(ff_db_ctrp __ctr, mdl_uint_t __pile, mdl_uint_t __slotno) {
	return ff_db_del_record(__ctr->sock, __ctr->key, __ctr->enckey, __pile, __slotno);
}

ffly_err_t
ff_db_ctr_write(ff_db_ctrp __ctr, mdl_uint_t __pile, mdl_uint_t __rec, mdl_u32_t __offset, void *__buf, mdl_uint_t __size) {
	return ff_db_write(__ctr->sock, __ctr->key, __ctr->enckey, __pile, __rec, __offset, __buf, __size);
}

ffly_err_t
ff_db_ctr_read(ff_db_ctrp __ctr, mdl_uint_t __pile, mdl_uint_t __rec, mdl_u32_t __offset, void *__buf, mdl_uint_t __size) {
	return ff_db_read(__ctr->sock, __ctr->key, __ctr->enckey, __pile, __rec, __offset, __buf, __size);
}

ffly_err_t
ff_db_ctr_record_alloc(ff_db_ctrp __ctr, mdl_uint_t __rec) {
	return ff_db_record_alloc(__ctr->sock, __ctr->key, __ctr->enckey, __rec);
}

ffly_err_t
ff_db_ctr_record_free(ff_db_ctrp __ctr, mdl_uint_t __rec) {
	return ff_db_record_free(__ctr->sock, __ctr->key, __ctr->enckey, __rec);
}

ffly_err_t
ff_db_ctr_rivet(ff_db_ctrp __ctr, mdl_u16_t __rivetno, mdl_uint_t __slotno) {
	return ff_db_rivet(__ctr->sock, __ctr->key, __ctr->enckey, __rivetno, __slotno);
}

ffly_err_t
ff_db_ctr_derivet(ff_db_ctrp __ctr, mdl_u16_t __rivetno) {
	return ff_db_derivet(__ctr->sock, __ctr->key, __ctr->enckey, __rivetno);
}

ffly_err_t
ff_db_ctr_rivetto(ff_db_ctrp __ctr, mdl_u16_t __rivetno, mdl_uint_t __slotno) {
	return ff_db_rivetto(__ctr->sock, __ctr->key, __ctr->enckey, __rivetno, __slotno);
}

ffly_err_t
ff_db_ctr_bind(ff_db_ctrp __ctr, mdl_u16_t __rivetno, mdl_uint_t __slotno, mdl_u8_t __offset) {
	return ff_db_bind(__ctr->sock, __ctr->key, __ctr->enckey, __rivetno, __slotno, __offset);
}

ffly_err_t
ff_db_ctr_acquire_slot(ff_db_ctrp __ctr, mdl_uint_t *__slotno) {
	return ff_db_acquire_slot(__ctr->sock, __ctr->key, __ctr->enckey, __slotno);
}

ffly_err_t
ff_db_ctr_scrap_slot(ff_db_ctrp __ctr, mdl_uint_t __slotno) {
	return ff_db_scrap_slot(__ctr->sock, __ctr->key, __ctr->enckey, __slotno);
}

mdl_i8_t
ff_db_ctr_exist(ff_db_ctrp __ctr, mdl_u16_t __rivetno, ffly_err_t *__err) {
	return ff_db_exist(__ctr->sock, __ctr->key, __ctr->enckey, __rivetno, __err);
}

mdl_i8_t static
ratifykey(FF_SOCKET *__sock, mdl_u8_t *__key, mdl_u64_t __enckey) {
	ffly_err_t err;
	ffly_err_t fault;
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

ffly_err_t
ff_db_shutdown(FF_SOCKET *__sock) {
	ffly_printf("shutdown.\n");
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_shutdown
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {

	}

	ff_db_rcv_err(__sock, &err);
	if (_err(err)) {
		ffly_printf("errstr: %s\n", ff_db_errst(get_errno(__sock, &err)));
		reterr;
	}
	retok;
}

ffly_err_t
ff_db_disconnect(FF_SOCKET *__sock) {
	ffly_printf("disconnect.\n");
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_disconnect
	};

	ffly_err_t err;
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
get_errno(FF_SOCKET *__sock, ffly_err_t *__err) {
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_req_errno
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {

	}

	ff_db_rcv_err(__sock, &err);
	if (_err(err)) {

	}

	ff_db_err ern;
	ff_db_rcv_errno(__sock, &ern);
	return ern;
}

ffly_err_t
ff_db_login(FF_SOCKET *__sock, mdl_u8_t const *__id, mdl_uint_t __il,
	mdl_u32_t __passkey, mdl_u8_t *__key, mdl_u64_t __enckey)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_login
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {
	
	}

	ff_db_rcv_err(__sock, &err);
	if (_err(err)) {
		ffly_printf("errstr: %s\n", ff_db_errst(get_errno(__sock, &err)));
		return err;
	}

	ffly_printf("sent message.\n");
	ff_net_send(__sock, &__il, sizeof(mdl_uint_t), &err);
	ffly_printf("sent id length.\n");

	ff_net_send(__sock, __id, __il, &err);
	ffly_printf("sent id.\n");
	ff_net_send(__sock, &__passkey, sizeof(mdl_u32_t), &err);
	ffly_printf("sent passkey.\n");

	ff_db_rcv_err(__sock, &err);
	if (_err(err)) {
		ffly_printf("errstr: %s\n", ff_db_errst(get_errno(__sock, &err)));
		return err;
	}

	ff_db_rcv_key(__sock, __key, __enckey);
	retok;
}

ffly_err_t
ff_db_logout(FF_SOCKET *__sock, mdl_u8_t *__key, mdl_u64_t __enckey) {
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_logout
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {
		ffly_printf("failed to send message.\n");
	} 

	ffly_err_t fault;
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

ffly_err_t
ff_db_creat_pile(FF_SOCKET *__sock, mdl_u8_t *__key,
	mdl_u64_t __enckey, mdl_uint_t *__slotno)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_creat_pile
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {
		ffly_printf("failed to send message.\n");
	}

	ffly_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {
		ffly_printf("failed to recv error.\n");
	}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_recv(__sock, __slotno, sizeof(mdl_uint_t), &err);
	if (_err(err)) {
		ffly_printf("failed to recv slotno.\n");
	}
	retok;
}

ffly_err_t
ff_db_del_pile(FF_SOCKET *__sock, mdl_u8_t *__key,
	mdl_u64_t __enckey, mdl_uint_t __slotno)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_del_pile
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {
		ffly_printf("failed to send message.\n");
	}

	ffly_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {
		ffly_printf("failed to recv error.\n");
	}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__slotno, sizeof(mdl_uint_t), &err);
	if (_err(err)) {
		ffly_printf("failed to send slotno.\n");
	}
	retok;
}

ffly_err_t
ff_db_creat_record(FF_SOCKET *__sock, mdl_u8_t *__key, mdl_u64_t __enckey,
	mdl_uint_t __pile, mdl_uint_t *__slotno, mdl_uint_t __size)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_creat_record
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {
		ffly_printf("failed to send message.\n");
	}

	ffly_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {
	}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__pile, sizeof(mdl_uint_t), &err);
	ff_net_send(__sock, &__size, sizeof(mdl_uint_t), &err);
	ff_net_recv(__sock, __slotno, sizeof(mdl_uint_t), &err);
	retok;
}

ffly_err_t
ff_db_del_record(FF_SOCKET *__sock, mdl_u8_t *__key, mdl_u64_t __enckey,
	mdl_uint_t __pile, mdl_uint_t __slotno)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_del_record
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {
		ffly_printf("failed to send message.\n");
	}

	ffly_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {
	}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__pile, sizeof(mdl_uint_t), &err);
	ff_net_send(__sock, &__slotno, sizeof(mdl_uint_t), &err);
	retok;
}

ffly_err_t
ff_db_write(FF_SOCKET *__sock, mdl_u8_t *__key, mdl_u64_t __enckey, mdl_uint_t __pile,
	mdl_uint_t __rec, mdl_u32_t __offset, void *__buf, mdl_uint_t __size)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_write
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__pile, sizeof(mdl_uint_t), &err);
	ff_net_send(__sock, &__rec, sizeof(mdl_uint_t), &err);
	ff_net_send(__sock, &__offset, sizeof(mdl_u32_t), &err);
	ff_net_send(__sock, &__size, sizeof(mdl_uint_t), &err);
	ff_net_send(__sock, __buf, __size, &err);
	retok;
}

ffly_err_t
ff_db_read(FF_SOCKET *__sock, mdl_u8_t *__key, mdl_u64_t __enckey, mdl_uint_t __pile,
	mdl_uint_t __rec, mdl_u32_t __offset, void *__buf, mdl_uint_t __size)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_read
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ffly_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__pile, sizeof(mdl_uint_t), &err);
	ff_net_send(__sock, &__rec, sizeof(mdl_uint_t), &err);
	ff_net_send(__sock, &__offset, sizeof(mdl_u32_t), &err);
	ff_net_send(__sock, &__size, sizeof(mdl_uint_t), &err);
	ff_net_recv(__sock, __buf, __size, &err);
	retok;
}

ffly_err_t
ff_db_record_alloc(FF_SOCKET *__sock, mdl_u8_t *__key,
	mdl_u64_t __enckey, mdl_uint_t __rec)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_record_alloc
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ffly_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__rec, sizeof(mdl_uint_t), &err);
	retok;
}

ffly_err_t
ff_db_record_free(FF_SOCKET *__sock, mdl_u8_t *__key,
	mdl_u64_t __enckey, mdl_uint_t __rec)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_record_free
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ffly_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__rec, sizeof(mdl_uint_t), &err);
	retok;
}

ffly_err_t
ff_db_rivet(FF_SOCKET *__sock, mdl_u8_t *__key, mdl_u64_t __enckey,
	mdl_u16_t __rivetno, mdl_uint_t __slotno)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_rivet
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ffly_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__slotno, sizeof(mdl_uint_t), &err);
	ff_net_send(__sock, &__rivetno, sizeof(mdl_u16_t), &err);
	retok;
}

ffly_err_t
ff_db_derivet(FF_SOCKET *__sock, mdl_u8_t *__key,
	mdl_u64_t __enckey, mdl_u16_t __rivetno)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_derivet
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ffly_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__rivetno, sizeof(mdl_u16_t), &err);
	retok;
}

ffly_err_t
ff_db_rivetto(FF_SOCKET *__sock, mdl_u8_t *__key, mdl_u64_t __enckey,
	mdl_u16_t __rivetno, mdl_uint_t __slotno)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_rivetto
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ffly_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__slotno, sizeof(mdl_uint_t), &err);
	ff_net_send(__sock, &__rivetno, sizeof(mdl_u16_t), &err);
	retok;
}

ffly_err_t
ff_db_bind(FF_SOCKET *__sock, mdl_u8_t *__key, mdl_u64_t __enckey,
	mdl_u16_t __rivetno, mdl_uint_t __slotno, mdl_u8_t __offset)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_bind
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ffly_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__slotno, sizeof(mdl_uint_t), &err);
	ff_net_send(__sock, &__rivetno, sizeof(mdl_u16_t), &err);
	ff_net_send(__sock, &__offset, sizeof(mdl_u8_t), &err);
	retok;
}

ffly_err_t 
ff_db_acquire_slot(FF_SOCKET *__sock, mdl_u8_t *__key,
	mdl_u64_t __enckey, mdl_uint_t *__slotno)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_acquire_slot
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ffly_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_recv(__sock, __slotno, sizeof(mdl_uint_t), &err);
	retok;
}

ffly_err_t
ff_db_scrap_slot(FF_SOCKET *__sock, mdl_u8_t *__key,
	mdl_u64_t __enckey, mdl_uint_t __slotno)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_scrap_slot
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ffly_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		ffly_printf("key was rejected.\n");
		reterr;
	}

	ff_net_send(__sock, &__slotno, sizeof(mdl_uint_t), &err);
	retok;
}

mdl_i8_t 
ff_db_exist(FF_SOCKET *__sock, mdl_u8_t *__key, mdl_u64_t __enckey,
	mdl_u16_t __rivetno, ffly_err_t *__err)
{
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_exist
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {}

	ffly_err_t fault;
	if (_err(err = ff_db_rcv_err(__sock, &fault))) {}

	if (ratifykey(__sock, __key, __enckey) == -1) {
		*__err = FFLY_FAILURE;
		return -1;
	}

	ff_net_send(__sock, &__rivetno, sizeof(mdl_u16_t), &err);

	mdl_i8_t ret;
	ff_net_recv(__sock, &ret, sizeof(mdl_u8_t), &err);
	return ret;
}
