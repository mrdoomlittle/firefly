# include "../db.h"
# include "../types.h"
# include "../system/io.h"
# include "../dep/bzero.h"
# include "../system/string.h"
# include "../system/err.h"
# include "../dep/mem_cmp.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../dep/str_len.h"
# include "../system/thread.h"
ff_uint_t acquire_slot();
void scrap_slot(ff_uint_t);
void *slotget(ff_uint_t);
void slotput(ff_uint_t, void*);

/*
	multiclient not done.
*/

ff_i8_t static
ratifykey(FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__errn, ff_err_t *__err, ff_u8_t *__key) {
	ff_db_key key;
	*__err = FFLY_SUCCESS;
	if (_err(*__err = ff_db_rcv_key(__sock, key, __user->enckey))) {
		ffly_printf("failed to recv key.\n");
		return -1;	
	}

	if (ffly_mem_cmp(key, __key, KEY_SIZE) == -1) {
		ffly_printf("any action is not permited unless key is valid.\n");
		return -1;
	}
	return 0;
}

ff_err_t static
ff_db_login(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp *__user, ff_db_err *__err, ff_u8_t *__key) {
	ffly_printf("logging in.\n");
	ff_err_t err;
	char id[40];
	ff_uint_t il;
 
	ff_net_recv(__sock, &il, sizeof(ff_uint_t), &err);
	if (il >= 40) {
		ffly_printf("id length too long.\n");
		reterr;
	}
	ffly_printf("recved id length.\n");

	ff_net_recv(__sock, id, il, &err);
	ffly_printf("recved id.\n");
	ff_u32_t passkey;
	ff_net_recv(__sock, &passkey, sizeof(ff_u32_t), &err);
	ffly_printf("idlen: %u\n", il);

	ffly_printf("id: ");
	ff_u8_t i = 0;
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

	if (!user->loggedin) {
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
	user->loggedin = 0;
	retok; 
}

ff_err_t static
ff_db_logout(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__err, ff_u8_t *__key) {
	ffly_printf("logging out.\n");
	ff_err_t err;
	if (!ratifykey(__sock, __user, __err, &err, __key)) {
		goto _succ;
	}

	if (_err(err))
		_ret;
	ffly_printf("can't permit action.\n");

_fail:
	ff_db_snd_err(__sock, FFLY_FAILURE);
	reterr;
_succ:
	ff_db_snd_err(__sock, FFLY_SUCCESS);
	*__err = _ff_err_null;
	__user->loggedin = -1;
	ffly_printf("logged out.\n");
	retok;
}

ff_err_t static
ff_db_creat_pile(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__err, ff_u8_t *__key) {
	ffly_printf("create pile.\n");
	ff_err_t err;
	if (ratifykey(__sock, __user, __err, &err, __key) == -1) {
		if (_err(err))
			_ret;
		ffly_printf("can't permit action.\n");
		goto _fail;
	}

	ff_db_snd_err(__sock, FFLY_SUCCESS);

	ff_uint_t slotno = acquire_slot();

	slotput(slotno, ffdb_creat_pile(&__d->db));
	ffly_printf("create pile at slotno: %u\n", slotno);

	ff_net_send(__sock, &slotno, sizeof(ff_uint_t), &err);	
	if (_err(err)) {
		ffly_printf("failed to send slotno.\n");
	}

	retok;
_fail:
	ff_db_snd_err(__sock, FFLY_FAILURE);
	reterr;
}

ff_err_t static
ff_db_del_pile(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__err, ff_u8_t *__key) {
	ffly_printf("delete pile.\n");
	ff_err_t err;
	if (ratifykey(__sock, __user, __err, &err, __key) == -1) {
		if (_err(err))
			_ret;
		ffly_printf("can't permit action.\n");
		goto _fail;
	}

	ff_db_snd_err(__sock, FFLY_SUCCESS);

	ff_uint_t slotno;
	ff_net_recv(__sock, &slotno, sizeof(ff_uint_t), &err);
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

ff_err_t static
ff_db_creat_record(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__err, ff_u8_t *__key) {
	ffly_printf("create record.\n");
	ff_err_t err;
	if (ratifykey(__sock, __user, __err, &err, __key) == -1) {
		if (_err(err))
			_ret;
		ffly_printf("can't permit action.\n");
		goto _fail;
	}

	ff_db_snd_err(__sock, FFLY_SUCCESS);
	ff_uint_t slotno;
	ffdb_pilep pile;
	ff_net_recv(__sock, &slotno, sizeof(ff_uint_t), &err);
	pile = (ffdb_pilep)slotget(slotno);

	ff_uint_t size;
	ff_net_recv(__sock, &size, sizeof(ff_uint_t), &err);

	slotno = acquire_slot();
	slotput(slotno, ffdb_creat_record(&__d->db, pile, size));
	ff_net_send(__sock, &slotno, sizeof(ff_uint_t), &err);	

	retok;
_fail:
	ff_db_snd_err(__sock, FFLY_FAILURE);
	reterr;
}

ff_err_t static
ff_db_del_record(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__err, ff_u8_t *__key) {
	ffly_printf("delete record.\n");
	ff_err_t err;
	if (ratifykey(__sock, __user, __err, &err, __key) == -1) {
		if (_err(err))
			_ret;
		ffly_printf("can't permit action.\n");
		goto _fail;
	}

	ff_db_snd_err(__sock, FFLY_SUCCESS);
	ff_uint_t slotno;
	ffdb_pilep pile;
	ff_net_recv(__sock, &slotno, sizeof(ff_uint_t), &err);
	pile = (ffdb_pilep)slotget(slotno);

	ffdb_recordp rec;
	ff_net_recv(__sock, &slotno, sizeof(ff_uint_t), &err);

	rec = (ffdb_recordp)slotget(slotno);

	ffdb_del_record(&__d->db, pile, rec);
	scrap_slot(slotno);

	retok;
_fail:
	ff_db_snd_err(__sock, FFLY_FAILURE);
	reterr;
}

ff_err_t static
ff_db_write(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__err, ff_u8_t *__key) {
	ffly_printf("write.\n");
	ff_err_t err;
	if (ratifykey(__sock, __user, __err, &err, __key) == -1) {
		if (_err(err))
			_ret;
		ffly_printf("can't permit action.\n");
		goto _fail;
	}

	ff_db_snd_err(__sock, FFLY_SUCCESS);
	ff_uint_t slotno;
	ffdb_pilep pile;
	ff_net_recv(__sock, &slotno, sizeof(ff_uint_t), &err);
	pile = (ffdb_pilep)slotget(slotno);

	ffdb_recordp rec;
	ff_net_recv(__sock, &slotno, sizeof(ff_uint_t), &err);
	rec = (ffdb_recordp)slotget(slotno);

	ff_u32_t offset;
	ff_net_recv(__sock, &offset, sizeof(ff_u32_t), &err);

	ff_uint_t size;
	ff_net_recv(__sock, &size, sizeof(ff_uint_t), &err);

	void *buf = __ffly_mem_alloc(size);
	ff_net_recv(__sock, buf, size, &err);

	ffdb_write(&__d->db, pile, rec, offset, buf, size);	

	__ffly_mem_free(buf);	
	retok;
_fail:
	ff_db_snd_err(__sock, FFLY_FAILURE);
	reterr;
}

ff_err_t static
ff_db_read(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__err, ff_u8_t *__key) {
	ffly_printf("read.\n");
	ff_err_t err;
	if (ratifykey(__sock, __user, __err, &err, __key) == -1) {
		if (_err(err))
			_ret;
		ffly_printf("can't permit action.\n");
		goto _fail;
	}

	ff_db_snd_err(__sock, FFLY_SUCCESS);
	ff_uint_t slotno;
	ffdb_pilep pile;
	ff_net_recv(__sock, &slotno, sizeof(ff_uint_t), &err);
	pile = (ffdb_pilep)slotget(slotno);

	ffdb_recordp rec;
	ff_net_recv(__sock, &slotno, sizeof(ff_uint_t), &err);
	rec = (ffdb_recordp)slotget(slotno);

	ff_u32_t offset;
	ff_net_recv(__sock, &offset, sizeof(ff_u32_t), &err);

	ff_uint_t size;
	ff_net_recv(__sock, &size, sizeof(ff_uint_t), &err);

	void *buf = __ffly_mem_alloc(size);
	ffdb_read(&__d->db, pile, rec, offset, buf, size);

	ff_net_send(__sock, buf, size, &err);
	__ffly_mem_free(buf);
	retok;
_fail:
	ff_db_snd_err(__sock, FFLY_FAILURE);
	reterr;
}

ff_err_t static
ff_db_record_alloc(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__err, ff_u8_t *__key) {
	ffly_printf("record alloc.\n");
	ff_err_t err;
	if (ratifykey(__sock, __user, __err, &err, __key) == -1) {
		if (_err(err))
			_ret;
		ffly_printf("can't permit action.\n");
		goto _fail;
	}

	ff_db_snd_err(__sock, FFLY_SUCCESS);
	ff_uint_t slotno;
	ffdb_recordp rec;
	ff_net_recv(__sock, &slotno, sizeof(ff_uint_t), &err);
	rec = (ffdb_recordp)slotget(slotno);

	ffdb_record_alloc(&__d->db, rec);
	retok;
_fail:
	ff_db_snd_err(__sock, FFLY_FAILURE);
	reterr;
}

ff_err_t static
ff_db_record_free(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__err, ff_u8_t *__key) {
	ffly_printf("record free.\n");
	ff_err_t err;
	if (ratifykey(__sock, __user, __err, &err, __key) == -1) {
		if (_err(err))
			_ret;
		ffly_printf("can't permit action.\n");
		goto _fail;
	}

	ff_db_snd_err(__sock, FFLY_SUCCESS);
	ff_uint_t slotno;
	ffdb_recordp rec;
	ff_net_recv(__sock, &slotno, sizeof(ff_uint_t), &err);
	rec = (ffdb_recordp)slotget(slotno);

	ffdb_record_free(&__d->db, rec);
	retok;
_fail:
	ff_db_snd_err(__sock, FFLY_FAILURE);
	reterr;
}

ff_err_t static
ff_db_rivet(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__err, ff_u8_t *__key) {
	ffly_printf("rivet.\n");
	ff_err_t err;
	if (ratifykey(__sock, __user, __err, &err, __key) == -1) {
		if (_err(err))
			_ret;
		ffly_printf("can't permit action.\n");
		goto _fail;
	}

	ff_db_snd_err(__sock, FFLY_SUCCESS);
	ff_uint_t slotno;
	ff_net_recv(__sock, &slotno, sizeof(ff_uint_t), &err);

	ff_u16_t rivetno;
	ff_net_recv(__sock, &rivetno, sizeof(ff_u16_t), &err);

	ffdb_rivet(rivetno, slotget(slotno));	
	retok;
_fail:
	ff_db_snd_err(__sock, FFLY_FAILURE);
	reterr;
}

ff_err_t static
ff_db_derivet(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__err, ff_u8_t *__key) {
	ffly_printf("derivet.\n");
	ff_err_t err;
	if (ratifykey(__sock, __user, __err, &err, __key) == -1) {
		if (_err(err))
			_ret;
		ffly_printf("can't permit action.\n");
		goto _fail;
	}

	ff_db_snd_err(__sock, FFLY_SUCCESS);
	ff_u16_t rivetno;
	ff_net_recv(__sock, &rivetno, sizeof(ff_u16_t), &err);

	ffdb_derivet(rivetno);
	retok;
_fail:
	ff_db_snd_err(__sock, FFLY_FAILURE);
	reterr;
}

ff_err_t static
ff_db_rivetto(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__err, ff_u8_t *__key) {
	ffly_printf("rivetto.\n");
	ff_err_t err;
	if (ratifykey(__sock, __user, __err, &err, __key) == -1) {
		if (_err(err))
			_ret;
		ffly_printf("can't permit action.\n");
		goto _fail;
	}

	ff_db_snd_err(__sock, FFLY_SUCCESS);
	ff_uint_t slotno;
	ff_net_recv(__sock, &slotno, sizeof(ff_uint_t), &err);

	ff_u16_t rivetno;
	ff_net_recv(__sock, &rivetno, sizeof(ff_u16_t), &err);

	slotput(slotno, ffdb_rivetto(rivetno));
	retok;
_fail:
	ff_db_snd_err(__sock, FFLY_FAILURE);
	reterr;
}

ff_err_t static
ff_db_bind(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__err, ff_u8_t *__key) {
	ffly_printf("bind.\n");
	ff_err_t err;
	if (ratifykey(__sock, __user, __err, &err, __key) == -1) {
		if (_err(err))
			_ret;
		ffly_printf("can't permit action.\n");
		goto _fail;
	}

	ff_db_snd_err(__sock, FFLY_SUCCESS);
	ff_uint_t slotno;
	ff_net_recv(__sock, &slotno, sizeof(ff_uint_t), &err);

	ff_u16_t rivetno;
	ff_net_recv(__sock, &rivetno, sizeof(ff_u16_t), &err);

	ff_u8_t offset;
	ff_net_recv(__sock, &offset, sizeof(ff_u8_t), &err);
	*(ff_u16_t*)((ff_u8_t*)slotget(slotno)+offset) = rivetno;
	retok;
_fail:
	ff_db_snd_err(__sock, FFLY_FAILURE);
	reterr;
}

ff_err_t static
ff_db_acquire_slot(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__err, ff_u8_t *__key) {
	ffly_printf("acquire slot.\n");
	ff_err_t err;
	if (ratifykey(__sock, __user, __err, &err, __key) == -1) {
		if (_err(err))
			_ret;
		ffly_printf("can't permit action.\n");
		goto _fail;
	}

	ff_db_snd_err(__sock, FFLY_SUCCESS);
	ff_uint_t slotno;

	slotno = acquire_slot();
	ff_net_send(__sock, &slotno, sizeof(ff_uint_t), &err);
	retok;
_fail:
	ff_db_snd_err(__sock, FFLY_FAILURE);
	reterr;
}

ff_err_t static
ff_db_scrap_slot(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__err, ff_u8_t *__key) {
	ffly_printf("scrap slot.\n");
	ff_err_t err;
	if (ratifykey(__sock, __user, __err, &err, __key) == -1) {
		if (_err(err))
			_ret;
		ffly_printf("can't permit action.\n");
		goto _fail;
	}

	ff_db_snd_err(__sock, FFLY_SUCCESS);
	ff_uint_t slotno;
	ff_net_recv(__sock, &slotno, sizeof(ff_uint_t), &err);

	scrap_slot(slotno);
	retok;
_fail:
	ff_db_snd_err(__sock, FFLY_FAILURE);
	reterr;
}

ff_err_t static
ff_db_exist(ff_dbdp __d, FF_SOCKET *__sock, ff_db_userp __user, ff_db_err *__err, ff_u8_t *__key) {
	ffly_printf("exist.\n");
	ff_err_t err;
	if (ratifykey(__sock, __user, __err, &err, __key) == -1) {
		if (_err(err))
			_ret;
		ffly_printf("can't permit action.\n");
		goto _fail;
	}

	ff_db_snd_err(__sock, FFLY_SUCCESS);

	ff_u16_t rivetno;
	ff_net_recv(__sock, &rivetno, sizeof(ff_u16_t), &err);

	ff_i8_t ret;
	
	ret = ffdb_exist(rivetno);
	ff_net_send(__sock, &ret, sizeof(ff_i8_t), &err);

	retok;
_fail:
	ff_db_snd_err(__sock, FFLY_FAILURE);
	reterr;
}

# include "../system/util/hash.h"
ff_u8_t cmdauth[] = {
	_ff_db_auth_null,	//login
	_ff_db_auth_null,	//logout
	_ff_db_auth_null,	//pulse
	_ff_db_auth_null,	//shutdown <- should be root only access
	_ff_db_auth_null,	//disconnect
	_ff_db_auth_null,	//req_errno
	_ff_db_auth_root,	//creat_pile
	_ff_db_auth_root,	//del_pile
	_ff_db_auth_root,	//creat_record
	_ff_db_auth_root,	//del_record
	_ff_db_auth_root,	//write
	_ff_db_auth_root,	//read
	_ff_db_auth_root,	//record_alloc
	_ff_db_auth_root,	//record_free
	_ff_db_auth_root,	//rivet
	_ff_db_auth_root,	//derivet
	_ff_db_auth_root,	//revitto
	_ff_db_auth_root,	//bind
	_ff_db_auth_root,	//acquire_slot
	_ff_db_auth_root,	//scrap_slot
	_ff_db_auth_root	//exist
};

ff_u8_t static
has_auth(ff_db_userp __user, ff_u8_t __cmd) {
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
void _ff_creat_record();
void _ff_del_record();
void _ff_write();
void _ff_read();
void _ff_record_alloc();
void _ff_record_free();
void _ff_rivet();
void _ff_derivet();
void _ff_rivetto();
void _ff_bind();
void _ff_acquire_slot();
void _ff_scrap_slot();
void _ff_exist();

void *jmp[] = {
	_ff_login,
	_ff_logout,
	_ff_pulse,
	_ff_shutdown,
	_ff_disconnect,
	_ff_req_errno,
	_ff_creat_pile,
	_ff_del_pile,
	_ff_creat_record,
	_ff_del_record,
	_ff_write,
	_ff_read,
	_ff_record_alloc,
	_ff_record_free,
	_ff_rivet,
	_ff_derivet,
	_ff_rivetto,
	_ff_bind,
	_ff_acquire_slot,
	_ff_scrap_slot,
	_ff_exist
};

char const *msgstr(ff_u8_t __kind) {
	switch(__kind) {
		case _ff_db_msg_login:		return "login";
		case _ff_db_msg_logout:		return "logout";
		case _ff_db_msg_pulse:		return "pulse";
		case _ff_db_msg_shutdown:	return "shutdown";
		case _ff_db_msg_disconnect:	return "disconnect";
		case _ff_db_msg_req_errno:	return "request error number";
		case _ff_db_msg_creat_pile:	return "create pile";
		case _ff_db_msg_del_pile:	return "delete pile";
		case _ff_db_msg_creat_record: return "create record";
		case _ff_db_msg_del_record:	return "delete record";
		case _ff_db_msg_write:		return "write";
		case _ff_db_msg_read:		return "read";
		case _ff_db_msg_record_alloc:	return "record alloc";
		case _ff_db_msg_record_free:	return "record free";
		case _ff_db_msg_rivet:		return "rivet";
		case _ff_db_msg_derivet:	return "derivet";
		case _ff_db_msg_rivetto:	return "rivetto";
		case _ff_db_msg_bind:		return "bind";
		case _ff_db_msg_acquire_slot:	return "acquire slot";
		case _ff_db_msg_scrap_slot:	return "scrap slot";
		case _ff_db_msg_exist:		return "exist";
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
# define jmpexit __asm__("jmp _ff_exit")
# include "../linux/types.h"
# include "../pellet.h"
# include "../ctl.h"
# include "client.h"
# include "../system/atomic.h"
ffly_potp static main_pot;
FF_SOCKET *sock;
ff_i8_t to_shut = -1;
ff_atomic_u32_t live = 0;
void ff_db_shut();

void*
ff_db_serve(void *__arg_p) {
	ffly_atomic_incr(&live);
	ffly_pelletp pel = (ffly_pelletp)__arg_p;
	ff_err_t err;
	ff_dbdp daemon;
	ff_i8_t alive;
	ff_db_err ern;
	ff_db_key key;
	ff_db_userp user = NULL;
	struct ff_db_msg msg;
	FF_SOCKET *peer;

	ffly_pellet_getd(pel, (void**)&daemon, sizeof(ff_dbdp));
	ffly_pellet_getd(pel, (void**)&peer, sizeof(FF_SOCKET*));
	ffly_pellet_free(pel);

	alive = 0;
	ffly_ctl(ffly_malc, _ar_setpot, (ff_u64_t)main_pot);	
	while(!alive && to_shut<0) {
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

		if (msg.kind > _ff_db_msg_exist) {
			jmpexit;
		}

		jmpto(jmp[msg.kind]);

		__asm__("_ff_creat_pile:\n\t"); {
			ff_db_creat_pile(daemon, peer, user, &ern, key);
		}
		jmpend;

		__asm__("_ff_del_pile:\n\t"); {
			_pr(&daemon->db);
			_pf();
			ff_db_del_pile(daemon, peer, user, &ern, key);
		}
		jmpend;

		__asm__("_ff_creat_record:\n\t"); {
			ff_db_creat_record(daemon, peer, user, &ern, key);
		}
		jmpend;

		__asm__("_ff_del_record:\n\t"); {
			ff_db_del_record(daemon, peer, user, &ern, key);
		}
		jmpend;

		__asm__("_ff_write:\n\t"); {
			ff_db_write(daemon, peer, user, &ern, key);
		}
		jmpend;

		__asm__("_ff_read:\n\t"); {
			ff_db_read(daemon, peer, user, &ern, key);
		}
		jmpend;

		__asm__("_ff_record_alloc:\n\t"); {
			ff_db_record_alloc(daemon, peer, user, &ern, key);
		}
		jmpend;

		__asm__("_ff_record_free:\n\t"); {
			ff_db_record_free(daemon, peer, user, &ern, key);
		}
		jmpend;

		__asm__("_ff_rivet:\n\t"); {
			ff_db_rivet(daemon, peer, user, &ern, key);
		}
		jmpend;

		__asm__("_ff_derivet:\n\t"); {
			ff_db_derivet(daemon, peer, user, &ern, key);
		}
		jmpend;

		__asm__("_ff_rivetto:\n\t"); {
			ff_db_rivetto(daemon, peer, user, &ern, key);
		}
		jmpend;

		__asm__("_ff_bind:\n\t"); {
			ff_db_bind(daemon, peer, user, &ern, key);
		}
		jmpend;

		__asm__("_ff_acquire_slot:\n\t"); {
			ff_db_acquire_slot(daemon, peer, user, &ern, key);
		}
		jmpend;

		__asm__("_ff_scrap_slot:\n\t"); {
			ff_db_scrap_slot(daemon, peer, user, &ern, key);
		}
		jmpend;

		__asm__("_ff_exist:\n\t"); {
			ff_db_exist(daemon, peer, user, &ern, key);
		}
		jmpend;

		__asm__("_ff_shutdown:\n\t"); {
			ffly_printf("goodbye.\n");
			ff_net_close(peer);
			ff_db_shut();
		}
		jmpexit;

		__asm__("_ff_pulse:\n\t"); {
			// echo
		}
		jmpend; 
	
		__asm__("_ff_login:\n\t"); {
			if (_err(ff_db_login(daemon, peer, &user, &ern, key)))
				ffly_printf("failed to login.\n");
		}
		jmpend;

		__asm__("_ff_logout:\n\t"); {
			if (_err(ff_db_logout(daemon, peer, user, &ern, key)))
				ffly_printf("failed to logout.\n");
			user = NULL;
		}
		jmpend;

		__asm__("_ff_disconnect:\n\t"); {
			ff_net_close(peer);
		}
		jmpexit;
	
		__asm__("_ff_req_errno:\n\t"); {
			ff_db_snd_errno(peer, ern);
		}
		jmpend;
		__asm__("_ff_end:\n\t");
		// do somthing
	}
	ff_net_close(peer);	
	__asm__("_ff_exit:\n\t");
	ffly_ctl(ffly_malc, _ar_unset, 0);
	ffly_atomic_decr(&live);
	return NULL;
}

void ff_db_shut() {
	to_shut = 0;
	ff_net_shutdown(sock, SHUT_RDWR);
}

void static
def_users(ff_dbdp __d) {
	char const *id[] = {"root", "mrdoomlittle", NULL};
	char const **cur_id = id;
	ff_db_userp user;
	while(*cur_id != NULL) {
		user = ff_db_add_user(__d, *cur_id, ffly_str_len(*cur_id), ffly_hash("none", 4));
		user->auth_level = _ff_db_auth_root;
		user->enckey = ffly_hash("firefly", 7);
		cur_id++;
	}
}

void
ff_dbd_start(char const *__file, ff_u16_t __port) {
	struct ff_dbd daemon;
	ffdb_init(&daemon.db);
	ffdb_open(&daemon.db, __file);
	daemon.list = (void**)__ffly_mem_alloc(0x100*sizeof(void*));
	ffly_map_init(&daemon.users, _ffly_map_127);

	def_users(&daemon);

	ff_err_t err;
	struct sockaddr_in addr, cl;
	ffly_bzero(&addr, sizeof(struct sockaddr_in));
	ffly_bzero(&cl, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_port = htons(__port);
	sock = ff_net_creat(&err, AF_INET, SOCK_STREAM, 0);
	int val = 1;
	setsockopt(sock->fd, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int));

	if (_err(ff_net_bind(sock, (struct sockaddr*)&addr, sizeof(struct sockaddr_in)))) {
		ff_net_close(sock);
		return;
	}

	socklen_t len;
	FF_SOCKET *peer;
	ffly_pelletp pel;
	void *p;
_again:
	if (_err(ff_net_listen(sock))) {
		ffly_printf("failed to listen.\n");
		goto _end;
	}

	len = sizeof(struct sockaddr_in);
	peer = ff_net_accept(sock, (struct sockaddr*)&cl, &len, &err);
	if (_err(err)) {
		ffly_printf("failed to accept client.\n");
		goto _end;
	}

	pel = ffly_pellet_mk(sizeof(FF_SOCKET*)+sizeof(ff_dbdp));
	ffly_pellet_puti(pel, (void**)&peer, sizeof(FF_SOCKET*));
	p = &daemon;
	ffly_pellet_puti(pel, (void**)&p, sizeof(ff_dbdp));
	ff_db_client(peer, pel);
	goto _again;
_end:
	ffly_printf("waiting for theads.\n");
	while(live>0);
	ff_net_close(sock);
	cleanup(&daemon);
	ffly_map_de_init(&daemon.users);
	__ffly_mem_free(daemon.list);
	ffdb_settle(&daemon.db);
	ffdb_cleanup(&daemon.db);
	ffdb_close(&daemon.db);
}

# include "../opt.h"
# include "../depart.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	char const *file = NULL;
	char const *port = NULL;
	struct ffpcll pcl;
	ffoe_prep();
	pcl.cur = __argv+1;
	pcl.end = __argv+__argc;
	ffoe(ffoe_pcll, (void*)&pcl);
	file = ffoptval(ffoe_get("f")) ;
	port = ffoptval(ffoe_get("p"));
	ffoe_end();

	if (!file || !port) {
		ffly_printf("usage: ./daemon -p #### -f ####.db\n");
		return -1;
	}

	ffly_ctl(ffly_malc, _ar_getpot, (ff_u64_t)&main_pot);
	ff_dbd_start(file, ffly_stno(port));	  
	ffly_depart(NULL);
	return 0;
}
