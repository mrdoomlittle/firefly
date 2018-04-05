# include "../db.h"
# include "../system/io.h"
# include "../system/err.h"
# include "../types/err_t.h"
# include "../dep/str_cmp.h"
# include "../system/string.h"
# include "../system/util/hash.h"
# include "../dep/bzero.h"
# include "../inet.h"
ff_db_err static get_errno(FF_SOCKET*, ffly_err_t*);

mdl_i8_t
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

ff_db_err
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
ff_db_login(FF_SOCKET *__sock, mdl_u8_t const *__id, mdl_uint_t __il, mdl_u32_t __passkey, mdl_u8_t *__key, mdl_u64_t __enckey) {
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
ff_db_creat_pile(FF_SOCKET *__sock, mdl_u8_t *__key, mdl_u64_t __enckey, mdl_uint_t *__slotno) {
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
ff_db_del_pile(FF_SOCKET *__sock, mdl_u8_t *__key, mdl_u64_t __enckey, mdl_uint_t __slotno) {
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
ff_db_derivet(FF_SOCKET *__sock, mdl_u8_t *__key, mdl_u64_t __enckey,
	mdl_u16_t __rivetno)
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

# include "../ffly_def.h"
# include "../dep/str_cpy.h"
# include "../system/nanosleep.h"
ffly_err_t ffmain(int __argc, char const *__argv[]) {
	char const **arg = __argv+1;
	char const *port = NULL;
	char const *ip = NULL;
	while(arg != __argv+__argc) {
		if (!ffly_str_cmp(*arg, "-port"))
			port = *(++arg);
		else if (!ffly_str_cmp(*arg, "-ip"))
			ip = *(++arg);
		arg++;
	}

	if (!port){
		ffly_printf("missing -port.\n");
		return -1;
	}

	if (!ip) {
		ffly_printf("missing -ip.\n");
		return -1;
	}

	ffly_printf("port: %s, address: %s\n", port, ip);

	ffly_err_t err;
	FF_SOCKET *sock = ff_net_creat(&err, AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in addr;
	ffly_bzero(&addr, sizeof(struct sockaddr_in));
	addr.sin_addr.s_addr = inet_addr(ip);
	addr.sin_family = AF_INET;
	addr.sin_port = htons(ffly_stno(port));
	ff_net_connect(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in));
	mdl_u8_t key[KEY_SIZE];

	mdl_u64_t enckey = 9331413;

	char const *uname = "root";
	if (_err(ff_db_login(sock, uname, ffly_str_len(uname), ffly_hash("21299", 5), key, enckey))) {
		ffly_printf("failed to login.\n");
	}

	if (ff_db_exist(sock, key, enckey, 0, &err) == -1) {
		mdl_uint_t pile, rec;
		if (_err(ff_db_creat_pile(sock, key, enckey, &pile))) {
			ffly_printf("failed to create pile.\n");
		}

		ff_db_rivet(sock, key, enckey, 0, pile);
		ff_db_bind(sock, key, enckey, 0, pile, offsetof(struct ffdb_pile, no));

		ffly_printf("pile, slotno: %u\n", pile);
		ff_db_creat_record(sock, key, enckey, pile, &rec, 20);	
		ff_db_rivet(sock, key, enckey, 1, rec);
		ff_db_bind(sock, key, enckey, 1, rec, offsetof(struct ffdb_record, no));

		ffly_printf("record, slotno: %u\n", rec);

		ff_db_record_alloc(sock, key, enckey, rec);
		char buf[67];
		ffly_str_cpy(buf, "https://github.com/mrdoomlittle/");
		ff_db_write(sock, key, enckey, pile, rec, 0, buf, 67);
	} else {
		ffly_printf("pile exists.\n");
		mdl_uint_t pile, rec;
		ff_db_acquire_slot(sock, key, enckey, &pile);
		ff_db_acquire_slot(sock, key, enckey, &rec);
		ff_db_rivetto(sock, key, enckey, 0, pile);
		ff_db_rivetto(sock, key, enckey, 1, rec);

		char buf[67];
		ff_db_read(sock, key, enckey, pile, rec, 0, buf, 67);
		ffly_printf("record: %s\n", buf);
	}

	if (_err(ff_db_logout(sock, key, enckey))) {
		ffly_printf("failed to logout.\n");
	}

	if (_err(ff_db_shutdown(sock))) {
		ffly_printf("failed to shutdown.\n");
	}
	
	ffly_nanosleep(1, 0);
	ff_net_close(sock);
	return 0;  
}
