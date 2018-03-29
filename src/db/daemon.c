# include "../db.h"
# include "../types/err_t.h"
# include "../system/io.h"
# include "../types/err_t.h"
# include "../dep/bzero.h"
# include "../system/string.h"
# include "../system/err.h"
# include "../dep/mem_cmp.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../dep/str_len.h"

# define NO_SLOTS 20

void static *slot[NO_SLOTS];
void static **fresh = slot;

mdl_uint_t static vacant[NO_SLOTS];
mdl_uint_t static *next = vacant;

mdl_uint_t acquire_slot() {
	if (next>vacant)
		return *(--next);
	return (fresh++)-slot;
}

void scrap_slot(mdl_uint_t __no) {
	if (__no>=NO_SLOTS)
		return;
	void **p = slot+__no;
	if (p+1 == fresh)
		fresh--;
	else
		*(next++) = __no;
}

void *slotget(mdl_uint_t __no) {
	if (__no>=NO_SLOTS)
		return NULL;
	return *(slot+__no);
}

void slotput(mdl_uint_t __no, void *__p) {
	if (__no>=NO_SLOTS)
		return;
	*(slot+__no) = __p;
}

ffly_err_t static
ff_db_login(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp *__user, ff_db_err *__err, mdl_u8_t *__key) {
	ffly_printf("logging in.\n");
	ffly_err_t err;
	char id[40];
	mdl_uint_t il;
 
	ff_net_recv(__sock, &il, sizeof(mdl_uint_t), &err);
	if (il >= 40) {
		ffly_printf("id length too long.\n");
		reterr;
	}
	ffly_printf("recved id length.\n");

	ff_net_recv(__sock, id, il, &err);
	ffly_printf("recved id.\n");
	mdl_u32_t passkey;
	ff_net_recv(__sock, &passkey, sizeof(mdl_u32_t), &err);
	ffly_printf("idlen: %u\n", il);

	ffly_printf("id: ");
	mdl_u8_t i = 0;
	while(i != il) {
		ffly_printf("%c", id[i]);
		i++;
	}
	ffly_printf("\n");

	ff_db_userp user = (*__user = ff_db_get_user(__d, id, il, &err)); 
	if (_err(err) || !user) {
		ffly_printf("user doesn't exist.\n");
		*__err = _ff_err_nsu;
		goto _fail;
	}

	if (passkey != user->passkey) {
		ffly_printf("passkey doesn't match.\n");
		*__err = _ff_err_lci;
		goto _fail;
	}

	if (user->loggedin) {
		ffly_printf("already logged in.\n");
		*__err = _ff_err_ali;
		goto _fail;
	}
	goto _succ;

	_fail:
	ff_db_snd_err(__sock, FFLY_FAILURE); 
	reterr;
	_succ:
	ff_db_snd_err(__sock, FFLY_SUCCESS);
	*__err = _ff_err_null;
	ff_db_keygen(__key);
	ff_db_add_key(__d, __key);
	ff_db_snd_key(__sock, __key, user->enckey);
	user->loggedin = 1;
	retok; 
}

ffly_err_t static
ff_db_logout(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__err, mdl_u8_t *__key) {
	ffly_printf("logging out.\n");
	ffly_err_t err;
	mdl_u8_t key[KEY_SIZE];
	if (_err(err = ff_db_rcv_key(__sock, key, __user->enckey))) {
		ffly_printf("failed to recv key.\n");
		_ret;
	}

	if (!ffly_mem_cmp(key, __key, KEY_SIZE)) {
		ff_db_rm_key(__d, __key); 
		__user->loggedin = 0;
		goto _succ;  
	}

	_fail:
	ff_db_snd_err(__sock, FFLY_FAILURE);
	reterr;
	_succ:
	ff_db_snd_err(__sock, FFLY_SUCCESS);
	*__err = _ff_err_null;
	ffly_printf("logged out.\n");
	retok;
}

ffly_err_t static
ff_db_creat_pile(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__err, mdl_u8_t *__key) {
	ffly_err_t err;
	ffdb_key key;
	if (_err(err = ff_db_rcv_key(__sock, key, __user->enckey))) {
		ffly_printf("failed to recv key.\n");
		_ret;
	}

	if (ffly_mem_cmp(key, __key, KEY_SIZE) == -1) {
		ffly_printf("key mismatch.\n");
		goto _fail;
	}

	ff_db_snd_err(__sock, FFLY_SUCCESS);

	mdl_uint_t slotno = acquire_slot();

	slotput(slotno, ffdb_creat_pile(&__d->db));
	ffly_printf("create pile at slotno: %u\n", slotno);

	ff_net_send(__sock, &slotno, sizeof(mdl_uint_t), &err);	
	if (_err(err)) {
		ffly_printf("failed to send slotno.\n");
	}

	retok;
	_fail:
	ff_db_snd_err(__sock, FFLY_FAILURE);
	reterr;
}

ffly_err_t static
ff_db_del_pile(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__err, mdl_u8_t *__key) {
	ffly_err_t err;
	ffdb_key key;
	if (_err(err = ff_db_rcv_key(__sock, key, __user->enckey))) {
		ffly_printf("failed to recv key.\n");
		_ret;
	}

	if (ffly_mem_cmp(key, __key, KEY_SIZE) == -1) {
		ffly_printf("key mismatch.\n");
		goto _fail;
	}

	ff_db_snd_err(__sock, FFLY_SUCCESS);

	mdl_uint_t slotno;
	ff_net_recv(__sock, &slotno, sizeof(mdl_uint_t), &err);
	if (_err(err)) {
		ffly_printf("failed to recv slotno.\n");
	}

	ffly_printf("delete pile at slotno: %u\n", slotno);
	ffdb_del_pile(&__d->db, slotget(slotno));
	scrap_slot(slotno);
	retok;
	_fail:
	ff_db_snd_err(__sock, FFLY_FAILURE);
	reterr;
}
/*
ffly_err_t static
ff_db_creat_record(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__err, mdl_u8_t *__key) {
	retok;
}

ffly_err_t static
ff_db_del_record(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__err, mdl_u8_t *__key) {
	retok;
}*/

# include "../system/util/hash.h"
mdl_u8_t cmdauth[] = {
	_ff_db_auth_null, //login
	_ff_db_auth_user, //logout
	_ff_db_auth_null, //pulse
	_ff_db_auth_null, //shutdown
	_ff_db_auth_null, //disconnect
	_ff_db_auth_null, //req_errno
	_ff_db_auth_null, //creat_pile
	_ff_db_auth_null  //del_pile
};

mdl_u8_t static
has_auth(ff_db_userp __user, mdl_u8_t __cmd) {
	if (!__user)
		return (cmdauth[__cmd] == _ff_db_auth_null);
	ffly_printf("root access: %s\n", __user->auth_level == _ff_db_auth_root?"yes":"no");
	return (__user->auth_level <= cmdauth[__cmd]);
}

void _ff_login();
void _ff_logout();
void _ff_pulse();
void _ff_shutdown();
void _ff_disconnect();
void _ff_req_errno();
void _ff_creat_pile();
void _ff_del_pile();

void *jmp[] = {
	_ff_login,
	_ff_logout,
	_ff_pulse,
	_ff_shutdown,
	_ff_disconnect,
	_ff_req_errno,
	_ff_creat_pile,
	_ff_del_pile
};

char const *msgstr(mdl_u8_t __kind) {
	switch(__kind) {
		case _ff_db_msg_login: return "login";
		case _ff_db_msg_logout: return "logout";
		case _ff_db_msg_pulse: return "pulse";
		case _ff_db_msg_shutdown: return "shutdown";
		case _ff_db_msg_disconnect: return "disconnect";
		case _ff_db_msg_req_errno: return "req_errno";
	}
	return "unknown";
}

void static
cleanup(ff_dbdp __daemon) {
	void const *cur = ffly_map_beg(&__daemon->users);
	while(cur != NULL) {
		ff_db_userp usr = (ff_db_userp)ffly_map_getp(cur);
		__ffly_mem_free(usr->id);
		ffly_map_fd(&__daemon->users, &cur);
	}
}

# define jmpto(__p) __asm__("jmp *%0" : : "r"(__p))
# define jmpend __asm__("jmp _ff_end")
# define jmpexit __asm__("jmp _ff_exit");
# include "../linux/types.h"
void
ff_dbd_start(mdl_u16_t __port) {
	struct ff_dbd daemon;
	ffdb_init(&daemon.db);
	ffdb_open(&daemon.db, "test.db");
	daemon.list = (void**)__ffly_mem_alloc((KEY_SIZE*0x100)*sizeof(void*));
	ffly_map_init(&daemon.users, _ffly_map_127);

	char const *rootid = "root";
	ff_db_userp root = ff_db_add_user(&daemon, rootid, ffly_str_len(rootid), ffly_hash("21299", 5));
	root->auth_level = _ff_db_auth_root; 
	root->enckey = 9331413;

	ffly_err_t err;
	struct sockaddr_in addr, cl;
	ffly_bzero(&addr, sizeof(struct sockaddr_in));
	ffly_bzero(&cl, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_port = htons(__port);
	FF_SOCKET *sock = ff_net_creat(&err, AF_INET, SOCK_STREAM, 0);
	int val = 1;
	setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int));
	
	if (_err(ff_net_bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)))) {
		ff_net_close(sock);
		return;
	}

	socklen_t len = sizeof(struct sockaddr_in);
	mdl_i8_t alive;
	ff_db_err ern;
	mdl_u8_t key[KEY_SIZE];
	ff_db_userp user = NULL;
	struct ff_db_msg msg;
	FF_SOCKET *peer;

	__asm__("_ff_bk:\nnop");
	ff_net_listen(sock);
	peer = ff_net_accept(sock, (struct sockaddr*)&cl, &len, &err);
	if (_err(err)) {
		ff_net_close(sock);
		return;
	}
	alive = 0;

	while(alive != -1) {
		if (_err(err = ff_db_rcvmsg(peer, &msg))) {
			ffly_printf("failed to recv message.\n");
			jmpexit;
		} 

		if (!has_auth(user, msg.kind)) {
			ffly_printf("user doesn't have authorization, for %s, user auth: %u\n", msgstr(msg.kind), !user?0:user->auth_level);
			ern = _ff_err_prd;
			ff_db_snd_err(peer, FFLY_FAILURE);
			continue;
		}

		if (_err(err = ff_db_snd_err(peer, FFLY_SUCCESS))) {
			jmpexit;
		}

		if (msg.kind > _ff_db_msg_del_pile) {
			jmpexit;
		}

		jmpto(jmp[msg.kind]);

		__asm__("_ff_creat_pile:\n\t"); {
			ff_db_creat_pile(&daemon, peer, user, &ern, key);
		}
		jmpend;

		__asm__("_ff_del_pile:\n\t"); {
			ff_db_del_pile(&daemon, peer, user, &ern, key);
		}
		jmpend;

		__asm__("_ff_creat_record:\n\t"); {
		}
		jmpend;

		__asm__("_ff_del_record:\n\t"); {
		}
		jmpend;

		__asm__("_ff_shutdown:\n\t"); {
			ffly_printf("goodbye.\n");
			ff_net_close(peer);    
		}
		jmpexit;

		__asm__("_ff_pulse:\n\t"); {
			// echo
		}
		jmpend; 
	
		__asm__("_ff_login:\n\t"); {
			if (_err(ff_db_login(&daemon, peer, &user, &ern, key)))
				ffly_printf("failed to login.\n");
		}
		jmpend;

		__asm__("_ff_logout:\n\t"); {
			if (_err(ff_db_logout(&daemon, peer, user, &ern, key)))
				ffly_printf("failed to logout.\n");
			user = NULL;
		}
		jmpend;

		__asm__("_ff_disconnect:\n\t"); {
			ff_net_close(peer);
		}
		__asm__("jmp _ff_bk");
	
		__asm__("_ff_req_errno:\n\t"); {
			ff_db_snd_errno(peer, ern);
		}
		jmpend;
		__asm__("_ff_end:\n\t");
		// do somthing
	}

	ffly_printf("somthing went wrong.\n");
	__asm__("_ff_exit:\n\t");
	ff_net_close(sock);
	cleanup(&daemon);
	ffly_map_de_init(&daemon.users);
	__ffly_mem_free(daemon.list);
	ffdb_cleanup(&daemon.db);
	ffdb_close(&daemon.db);
}

ffly_err_t ffmain(int __argc, char const *__argv[]) {
	if (__argc != 2) {
		ffly_printf("usage: deamon {port number}.\n");
		return -1;
	}

	char const *port = __argv[1];
	ff_dbd_start(ffly_stno(port));	  
	return 0;
}
