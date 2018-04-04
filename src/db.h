# ifndef __ffly__db__h
# define __ffly__db__h
# include <mdlint.h>
# include "net.h"
# include "types/err_t.h"
# include "types/off_t.h"
# include "system/map.h"
# include "system/file.h"
# include "system/mutex.h"
# define KEY_SIZE 2 // bytes
# define ffdb_no(__p) \
		(__p)->no
// bind rivet to pile or record
# define ffdb_bind(__p, __no) \
		(__p)->no = __no
// needs testing
enum {
	_ff_db_auth_root,
	_ff_db_auth_user,
	_ff_db_auth_null
};

enum {
	_ff_db_msg_login,
	_ff_db_msg_logout,
	_ff_db_msg_pulse,
	_ff_db_msg_shutdown,
	_ff_db_msg_disconnect,
	_ff_db_msg_req_errno,
	_ff_db_msg_creat_pile,
	_ff_db_msg_del_pile,
	_ff_db_msg_creat_record,
	_ff_db_msg_del_record
};

/*
	put this within define so only db.c and files that define somthing may be allowed to access,
	so with err value dont mix
*/
enum {
	_ff_err_null,
	_ff_err_lci,
	_ff_err_nsu,
	_ff_err_ali,
	_ff_err_prd
};

typedef mdl_u8_t ffdb_key[KEY_SIZE];

typedef mdl_u8_t ff_db_err;
# define MAX_PILES 20
struct ffdb_blkd {
	mdl_uint_t size;
	mdl_u32_t end, off;
	struct ffdb_blkd *fd, *bk, *p;
	mdl_u64_t prev, next;
	mdl_u8_t inuse;
};

// rethink this 'msg/kind'
struct ff_db_msg {
	mdl_u8_t kind;	 
};

struct ff_db_rep {
	mdl_u8_t type;
};

/*
	hold the underworld and surface together
*/
typedef struct ffdb_rivet {
	struct ffdb_rivet *next;
	mdl_u16_t no;
	void *to;
} *ffdb_rivetp;

struct ffdb_pile;
struct ffdb_record;

struct ffdb {
	FF_FILE *file;
	mdl_u32_t off;
	ffly_mutex_t lock;
	struct ffdb_pile *top, *free[20], **next;
	struct ffly_map map;
};

typedef struct ff_db_user {
	mdl_u8_t const *id;
	mdl_u32_t passkey;
	mdl_u8_t loggedin;
	mdl_u8_t auth_level;
	mdl_u64_t enckey;
} *ff_db_userp;

typedef struct ff_dbd {
	void **list;
	struct ffly_map users; 
	struct ffdb db;
} *ff_dbdp;

typedef struct ffdb_pile {
	struct ffly_map map; 
	struct ffdb_record *top;
	struct ffdb_pile *prev, *next;
	ffly_mutex_t lock;
	mdl_u16_t no;
} *ffdb_pilep;

typedef struct ffdb_record {
	mdl_u32_t p;
	struct ffdb_record *prev, *next;
	mdl_u16_t no;
} *ffdb_recordp;

typedef struct ffdb_blkd* ffdb_blkdp;

typedef struct ffdb* ffdbp;
typedef struct ff_db_msg* ff_db_msgp;

void ff_db_keygen(mdl_u8_t*);
void ff_db_add_key(ff_dbdp, mdl_u8_t*);
void ff_db_rm_key(ff_dbdp, mdl_u8_t*);
mdl_u8_t ff_db_valid_key(ff_dbdp, mdl_u8_t*);

ff_db_userp ff_db_get_user(ff_dbdp, mdl_u8_t const*, mdl_uint_t, ffly_err_t*);
ff_db_userp ff_db_add_user(ff_dbdp, mdl_u8_t const*, mdl_uint_t, mdl_u32_t);
void ff_db_del_user(ff_dbdp, mdl_u8_t const*, mdl_uint_t, mdl_u32_t);

ffly_err_t ff_db_snd_key(FF_SOCKET*, mdl_u8_t*, mdl_u64_t);
ffly_err_t ff_db_rcv_key(FF_SOCKET*, mdl_u8_t*, mdl_u64_t);

char const* ff_db_errst(ff_db_err);
ffly_err_t ff_db_snd_err(FF_SOCKET*, ffly_err_t);
ffly_err_t ff_db_rcv_err(FF_SOCKET*, ffly_err_t*);

ffly_err_t ff_db_snd_errno(FF_SOCKET*, ff_db_err);
ffly_err_t ff_db_rcv_errno(FF_SOCKET*, ff_db_err*);
ffly_err_t ff_db_sndmsg(FF_SOCKET*, ff_db_msgp);
ffly_err_t ff_db_rcvmsg(FF_SOCKET*, ff_db_msgp);
ffdb_recordp ffdb_creat_record(ffdbp, ffdb_pilep, mdl_uint_t);
ffdb_recordp ffdb_fetch_record(ffdbp, char const*, char const*);
void ffdb_del_record(ffdbp, ffdb_pilep, ffdb_recordp);
void ffdb_write(ffdbp, ffdb_pilep, ffdb_recordp, ffly_off_t, void*, mdl_uint_t);
void ffdb_read(ffdbp, ffdb_pilep, ffdb_recordp, ffly_off_t, void*, mdl_uint_t);

ffdb_pilep ffdb_creat_pile(ffdbp);
void ffdb_record_alias(ffdbp, ffdb_pilep, char const*, ffdb_recordp); 
void ffdb_pile_alias(ffdbp, char const*, ffdb_pilep);
ffdb_pilep ffdb_fetch_pile(ffdbp, char const*);
void ffdb_del_pile(ffdbp, ffdb_pilep);

mdl_uint_t ffdb_alloc(ffdbp, mdl_uint_t);
void ffdb_free(ffdbp, mdl_uint_t);
ffly_err_t ffdb_init(ffdbp);
ffly_err_t ffdb_cleanup(ffdbp);
ffly_err_t ffdb_open(ffdbp, char const*);
ffly_err_t ffdb_close(ffdbp);
# endif /*__ffly__db__h*/
