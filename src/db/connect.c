# include "../db.h"
# include "../system/io.h"
# include "../system/err.h"
# include "../types/err_t.h"
# include "../dep/str_cmp.h"
# include "../system/string.h"
# include "../system/util/hash.h"
# include "../dep/bzero.h"
# include "../inet.h"
ff_db_errno static get_errno(FF_SOCKET*, ffly_err_t*);

void ff_db_shutdown(FF_SOCKET *__sock) {
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_shutdown
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {

	}

	ff_db_rcv_err(__sock, &err);
	if (_err(err)) {
		ffly_printf("errstr: %s\n", ff_db_errno_str(get_errno(__sock, &err)));
		return;
	}
}

ff_db_errno get_errno(FF_SOCKET *__sock, ffly_err_t *__err) {
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_req_errno
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {

	}

	ff_db_rcv_err(__sock, &err);
	if (_err(err)) {

	}

	ff_db_errno ern;
	ff_db_rcv_errno(__sock, &ern);
	return ern;
}

ffly_err_t ff_db_login(FF_SOCKET *__sock, mdl_u8_t const *__id, mdl_uint_t __il, mdl_u32_t __passkey, mdl_u8_t *__key, mdl_u64_t __enckey) {
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_login
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {
	
	}

	ff_db_rcv_err(__sock, &err);
	if (_err(err)) {
		ffly_printf("errstr: %s\n", ff_db_errno_str(get_errno(__sock, &err)));
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
		ffly_printf("errstr: %s\n", ff_db_errno_str(get_errno(__sock, &err)));
		return err;
	}

	ff_db_rcv_key(__sock, __key, __enckey);
	retok;
}

ffly_err_t ff_db_logout(FF_SOCKET *__sock, mdl_u8_t *__key, mdl_u64_t __enckey) {
	struct ff_db_msg msg = {
		.kind = _ff_db_msg_logout
	};

	ffly_err_t err;
	if (_err(err = ff_db_sndmsg(__sock, &msg))) {

	} 

	ff_db_rcv_err(__sock, &err);
	if (_err(err)) {
		ffly_printf("errstr: %s\n", ff_db_errno_str(get_errno(__sock, &err)));
		return err;
	}

	ff_db_snd_key(__sock, __key, __enckey);

	ff_db_rcv_err(__sock, &err);
	if (_err(err)) {
		ff_db_errno ern = get_errno(__sock, &err);
		ffly_printf("errstr: %s\n", ff_db_errno_str(ern));
		return err;
	}
}

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

	char const *uname = "root";
	ff_db_login(sock, uname, ffly_str_len(uname), ffly_hash("21299", 5), key, 9331413);


	ff_db_logout(sock, key, 9331413);
	ff_db_shutdown(sock);
	
	ff_net_close(sock);
	return 0;  
}
