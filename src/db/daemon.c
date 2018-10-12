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
# include "../signal.h"
# include "../crypto.h"
ff_uint_t acquire_slot();
void scrap_slot(ff_uint_t);
void *slotget(ff_uint_t);
void slotput(ff_uint_t, void*);


ff_u8_t static stack[512];
# define stackat(__ad) \
	(stack+(__ad))

static ff_u8_t *db_cc;
static ff_dbdp d;

#define KEY_GOOD 0x01
struct {
	FF_SOCKET *sock;
	ff_db_userp user;
	ff_u8_t key[KEY_SIZE];
	ff_db_err *ern;
	ff_err_t err;
	ff_u8_t flags;
} client;
/*
	multiclient not done.
	TODO:
		move ff_dbdp
		static ffdbp d;
		dont pass as arg to function

		rename to dserver

		move most to exec.c or run.c 
*/

ff_i8_t static
ratifykey(void) {
	ff_err_t err;
	ff_u8_t key[KEY_SIZE];
	ffly_mem_cpy(key, db_cc, KEY_SIZE);
	ffly_decrypt(key, KEY_SIZE, client.user->enckey);
	ff_i8_t res;
	if (ffly_mem_cmp(key, client.key, KEY_SIZE) == -1) {
		ffly_printf("any action is not permited unless key is valid.\n");
		res = -1;
		goto _end;
	}
	res = 0;
	client.flags |= KEY_GOOD;
_end:
	ff_net_send(client.sock, &res, 1, 0, &err);
	return res;
}

ff_i8_t static
valid_key(void) {
	ff_i8_t res;
	res = ((ff_i8_t)((client.flags&KEY_GOOD)>0))-1;
	client.flags &= ~KEY_GOOD;
	return res;
}

void static
ff_db_login(void) {
	ffly_printf("logging in.\n");
	ff_err_t err;
	ff_u16_t ia, il;
	ia = *(ff_u16_t*)db_cc;

	char id[41];
	if ((il = *(ff_u16_t*)(db_cc+2)) >= 40) {
		ffly_printf("id length too long.\n");
		return;
	}
	ffly_printf("id; length: %u, stackloc: %u\n", il, ia);
	ffly_mem_cpy(id, stackat(ia), il);

	ff_u32_t passkey;
	passkey = *(ff_u32_t*)(db_cc+4);

	id[il] = '\0';
	ffly_printf("id: %s\n", id);

	ff_db_userp user;

	user = (client.user = ff_db_get_user(d, id, il, &err));
	if (_err(err) || !user) {
		ffly_printf("user doesn't exist.\n");
		goto _fail;
	}

	if (passkey != user->passkey) {
		ffly_printf("passkey doesn't match.\n");
		goto _fail;
	}

	if (!user->loggedin) {
		ffly_printf("user already logged in.");
		goto _fail;
	}
	goto _succ;
_fail:
	ffly_printf("login failure.\n");
	return;
_succ:
	ffly_printf("login success.\n");
	ff_db_keygen(client.key);
	ff_db_add_key(d, client.key);
	ff_u8_t key[KEY_SIZE];
	ffly_mem_cpy(key, client.key, KEY_SIZE);
	ffly_encrypt(key, KEY_SIZE, client.user->enckey);
	ff_net_send(client.sock, key, KEY_SIZE, 0, &err);
	user->loggedin = 0;
	ffly_fdrain(ffly_out);
}

void static
ff_db_logout(void) {
	ffly_printf("logging out.\n");
	ff_err_t err;
	if (!valid_key()) {
		goto _succ;
	}

	ffly_printf("can't permit action.\n");
_fail:

	return;
_succ:
	client.user->loggedin = -1;
	ffly_printf("logged out.\n");
}

void static
ff_db_pile_creat(void) {
	ffly_printf("pile create.\n");
	ff_err_t err;
	if (valid_key() == -1) {
		goto _fail;
	}

	ff_u32_t slot;
	slot = acquire_slot();
	slotput(slot, ffdb_pile_creat(&d->db));
	ffly_printf("created pile stored at slot %u\n", slot);
	ff_net_send(client.sock, &slot, sizeof(ff_u32_t), 0, &err);
	return;
_fail:
	return;
}

void static
ff_db_pile_del(void) {
	ffly_printf("pile delete.\n");
	if (valid_key() == -1) {
		goto _fail;
	}

	ff_u32_t slot;
	slot = *(ff_u32_t*)db_cc;
	ffly_printf("delete pile at slot: %u\n", slot);
	ffdb_pile_del(&d->db, slotget(slot));
	scrap_slot(slot);
	return;
_fail:
	return;
}

void static
ff_db_record_creat(void) {
	ffly_printf("record create.\n");
	if (valid_key() == -1) {
		goto _fail;
	}

	ff_err_t err;
	ff_u32_t slot;
	slot = *(ff_u32_t*)db_cc;
	ffdb_pilep pile;
	pile = (ffdb_pilep)slotget(slot);

	ff_u32_t size;
	size = *(ff_u32_t*)(db_cc+4);
	slot = acquire_slot();
	slotput(slot, ffdb_record_creat(&d->db, pile, size));
	ff_net_send(client.sock, &slot, sizeof(ff_u32_t), 0, &err);
	return;
_fail:
	return;
}

void static
ff_db_record_del(void) {
	ffly_printf("delete record.\n");
	if (valid_key() == -1) {
		goto _fail;
	}

	ff_u32_t slot;
	ffdb_pilep pile;
	ffdb_recordp rec;
	slot = *(ff_u32_t*)db_cc;
	pile = (ffdb_pilep)slotget(slot);
	slot = *(ff_u32_t*)(db_cc+4);
	rec = (ffdb_recordp)slotget(slot);
	ffdb_record_del(&d->db, pile, rec);
	scrap_slot(slot);
	return;
_fail:
	return;
}

void static
ff_db_write(void) {
	ffly_printf("write.\n");
	if (valid_key() == -1) {
		goto _fail;
	}
	ff_err_t err;
	ff_u32_t slot;
	ffdb_pilep pile;
	ffdb_recordp rec;
	slot = *(ff_u32_t*)db_cc;
	pile = (ffdb_pilep)slotget(slot);
	slot = *(ff_u32_t*)(db_cc+4);
	rec = (ffdb_recordp)slotget(slot);

	ff_u32_t offset, size;
	offset = *(ff_u32_t*)(db_cc+8);
	size = *(ff_u32_t*)(db_cc+12);

	void *buf;
	/*
		TODO:
			add max buf size
	*/
	buf = __ffly_mem_alloc(size);
	ff_net_recv(client.sock, buf, size, 0, &err);
	ffly_printf("offset: %u, size: %u\n", offset, size);
	ffdb_write(&d->db, pile, rec, offset, buf, size);
	__ffly_mem_free(size);
	return;
_fail:
	return;
}

void static
ff_db_read(void) {
	ffly_printf("read.\n");
	if (valid_key() == -1) {
		goto _fail;
	}
	ff_err_t err;
	ff_u32_t slot;
	ffdb_pilep pile;
	ffdb_recordp rec;
	slot = *(ff_u32_t*)db_cc;
	pile = (ffdb_pilep)slotget(slot);
	slot = *(ff_u32_t*)(db_cc+4);
	rec = (ffdb_recordp)slotget(slot);

	ff_u32_t offset, size;
	offset = *(ff_u32_t*)(db_cc+8);
	size = *(ff_u32_t*)(db_cc+12);

	void *buf;
	/*
		TODO:
			add max buf size
	*/
	buf = __ffly_mem_alloc(size);
	ffly_printf("offset: %u, size: %u\n", offset, size);
	ffdb_read(&d->db, pile, rec, offset, buf, size);
	ff_net_send(client.sock, buf, size, 0, &err);
	__ffly_mem_free(size);
	return;
_fail:
	return;
}

void static
ff_db_record_alloc(void) {
	ffly_printf("record allocate.\n");
	if (valid_key() == -1) {
		goto _fail;
	}
	ff_u32_t slot;
	slot = *(ff_u32_t*)db_cc;
	ffdb_recordp rec;
	rec = (ffdb_recordp)slotget(slot);
	ffdb_record_alloc(&d->db, rec);
	return;
_fail:
	return;
}

void static
ff_db_record_free(void) {
	ffly_printf("record free.\n");
	if (valid_key() == -1) {
		goto _fail;
	}
	ff_u32_t slot;
	slot = *(ff_u32_t*)db_cc;
	ffdb_recordp rec;
	rec = (ffdb_recordp)slotget(slot);
	ffdb_record_free(&d->db, rec);
	return;
_fail:
	return;
}

void static
ff_db_rivet(void) {
	ffly_printf("rivet.\n");
	if (valid_key() == -1) {
		goto _fail;
	}

	ff_u32_t slot;
	slot = *(ff_u32_t*)db_cc;
	ff_u16_t rivet;
	rivet = *(ff_u16_t*)(db_cc+4);
	ffdb_rivet(rivet, slotget(slot));
	return;
_fail:
	return;
}

void static
ff_db_derivet(void) {
	ffly_printf("derivet.\n");
	if (valid_key() == -1) {
		goto _fail;
	}
	ff_u16_t rivet;
	rivet = *(ff_u16_t*)db_cc;
	ffdb_derivet(rivet);
	return;
_fail:
	return;
}

void static
ff_db_rivetto(void) {
	ffly_printf("rivetto.\n");
	if (valid_key() == -1) {
		goto _fail;
	}
	ff_u32_t slot;
	ff_u16_t rivet;
	slot = *(ff_u32_t*)db_cc;
	rivet = *(ff_u16_t*)(db_cc+4);
	slotput(slot, ffdb_rivetto(rivet));
	return;
_fail:
	return;
}

void static
ff_db_bind(void) {
	ffly_printf("bind.\n");
	if (valid_key() == -1) {
		goto _fail;
	}

	ff_u32_t slot;
	ff_u16_t rivet;
	ff_u8_t offset;
	slot = *(ff_u32_t*)db_cc;
	rivet = *(ff_u16_t*)(db_cc+4);
	offset = *(db_cc+6);
	// big security issue
	*(ff_u16_t*)((ff_u8_t*)slotget(slot)+offset) = rivet;
	return;
_fail:
	return;
}

void static
ff_db_acquire_slot(void) {
	ffly_printf("acquire slot.\n");
	if (valid_key() == -1) {
		goto _fail;
	}
	ff_err_t err;
	ff_u32_t slot;
	slot = acquire_slot();
	ff_net_send(client.sock, &slot, sizeof(ff_u32_t), 0, &err);
	return;
_fail:
	return;
}

void static
ff_db_scrap_slot(void) {
	if (valid_key() == -1) {
		goto _fail;
	}
	scrap_slot(*(ff_u32_t*)db_cc);
	return;
_fail:
	return;
}

void static
ff_db_exist(void) {
	if (valid_key() == -1) {
		goto _fail;
	}
	ff_err_t err;
	ff_u16_t rivet;
	rivet = *(ff_u16_t*)db_cc;
	ff_i8_t r;
	r = ffdb_exist(rivet);
	ff_net_send(client.sock, &r, sizeof(ff_i8_t), 0, &err);
	return;
_fail:
	return;
}

void static
ff_db_record_stat(void) {
	if (valid_key() == -1) {
		goto _fail;
	}

	ff_err_t err;
	ff_u32_t slot;
	ffdb_recordp rec;
	slot = *(ff_u32_t*)db_cc;

	rec = (ffdb_recordp)slotget(slot);
	struct ffdb_recstat st;
	ffdb_record_stat(&d->db, rec, &st);
	ff_net_send(client.sock, &st, sizeof(struct ffdb_recstat), 0, &err);

	return;
_fail:
	return;
}

void static
ff_db_store(void) {
	ff_u16_t dst, n;
	dst = *(ff_u16_t*)db_cc;
	n = *(ff_u16_t*)(db_cc+2);
	ffly_printf("%u, %u\n", dst, n);
	ff_err_t err;
	ff_net_recv(client.sock, stackat(dst), n, 0, &err);
}

void static
ff_db_load(void) {
	ff_u16_t src, n;
	src = *(ff_u16_t*)db_cc;
	n = *(ff_u16_t*)(db_cc+2);

	ff_err_t err;
	ff_net_send(client.sock, stackat(src), n, 0, &err);
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
	_ff_db_auth_root,	//exist
	_ff_db_auth_root
};

ff_u8_t static
has_auth(ff_db_userp __user, ff_u8_t __cmd) {
	if (!__user)
		return (cmdauth[__cmd] == _ff_db_auth_null);
	ffly_printf("root access: %s\n", __user->auth_level == _ff_db_auth_root?"yes":"no");
	return (__user->auth_level <= cmdauth[__cmd]);
}

void ff_db_shut();
static void(*op[])(void) = {
	ff_db_login,
	ff_db_logout,
	ff_db_store,
	ff_db_load,
	ratifykey,
	ff_db_shut,
	ff_db_pile_creat,
	ff_db_pile_del,
	ff_db_record_creat,
	ff_db_record_del,
	ff_db_write,
	ff_db_read,
	ff_db_record_alloc,
	ff_db_record_free,
	ff_db_rivet,
	ff_db_derivet,
	ff_db_rivetto,
	ff_db_bind,
	ff_db_acquire_slot,
	ff_db_scrap_slot,
	ff_db_exist,
	ff_db_record_stat
};

static ff_uint_t osz[] = {
	_FF_LOGIN_S,
	_FF_LOGOUT_S,
	_FF_STORE_S,
	_FF_LOAD_S,
	_FF_RATIFYKEY_S,
	_FF_SHUTDOWN_S,
	_FF_PILE_CREAT_S,
	_FF_PILE_DEL_S,
	_FF_RECORD_CREAT_S,
	_FF_RECORD_DEL_S,
	_FF_WRITE_S,
	_FF_READ_S,
	_FF_RECORD_ALLOC_S,
	_FF_RECORD_FREE_S,
	_FF_RIVET_S,
	_FF_DERIVET_S,
	_FF_RIVETTO_S,
	_FF_BIND_S,
	_FF_ACQUIRE_SLOT_S,
	_FF_SCRAP_SLOT_S,
	_FF_EXIST_S,
	_FF_RECORD_STAT_S
};

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
int static sockfd;

void static sig(int __sig) {
	to_shut = 0;

	shutdown(sockfd, SHUT_RDWR);
}

/*
	NOTE:
		to be split up 
		e.g. asm
		_func0: - large routines

		------ small routines
		_store:
		_load:
*/
void static
texec(ff_db_tapep __t) {
	ff_u8_t *end;
	db_cc = (ff_u8_t*)__t->text;
	end = db_cc+__t->len;
	ff_u8_t on;
_again:
	if (db_cc>=end)
		return;
	on = *(db_cc++);
	ffly_printf("op: %u\n", on);
	op[on]();
	db_cc+=osz[on];
	goto _again;
}


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
	d = daemon;
	alive = 0;
//	ffly_ctl(ffly_malc, _ar_setpot, (ff_u64_t)main_pot);	
	client.sock = peer;
	ff_db_tapep t;
	while(!alive && to_shut<0) {
		ff_u32_t tsz;
		ff_net_recv(peer, &tsz, sizeof(ff_u32_t), 0, &err);		
		t = ff_db_tape_new(tsz);
		ff_net_recv(peer, t->text, tsz, 0, &err);
		ffly_printf("tape size: %u\n", tsz);
		texec(t);

		ff_db_tape_destroy(t);
	}
	ff_net_close(peer);	
	__asm__("_ff_exit:\n\t");
//	ffly_ctl(ffly_malc, _ar_unset, 0);
	ffly_atomic_decr(&live);
	return NULL;
}

void ff_db_shut(void) {
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
	struct sigaction sa;
	ffly_bzero(&sa, sizeof(struct sigaction));
	sa.sa_handler = sig;
	sigaction(SIGINT, &sa, NULL); 

	struct ff_dbd daemon;
	ffdb_init(&daemon.db);
	ffdb_open(&daemon.db, __file);
	daemon.list = (void**)__ffly_mem_alloc(0x100*sizeof(void*));
	void **cur = daemon.list;
	while(cur != daemon.list+0x100)
		*(cur++) = NULL;

	ffly_map_init(&daemon.users, _ffly_map_127);

	def_users(&daemon);

	ff_err_t err;
	struct sockaddr_in addr, cl;
	ffly_bzero(&addr, sizeof(struct sockaddr_in));
	ffly_bzero(&cl, sizeof(struct sockaddr_in));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htons(INADDR_ANY);
	addr.sin_port = htons(__port);
	sock = ff_net_creat(&err, _NET_PROT_TCP);

	// get socket fd
	sock->prot.get(0x00, (long long)&sockfd, sock->prot_ctx);
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
//	ff_db_client(peer, pel);
	ff_db_serve(pel);
//	if (to_shut == -1)
//		goto _again;
_end:
//	ffly_printf("waiting for theads.\n");
//	while(live>0);
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
