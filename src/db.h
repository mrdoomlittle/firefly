# ifndef __ffly__db__h
# define __ffly__db__h
# include "ffint.h"
# include "types.h"
# include "net.h"
# include "system/map.h"
# include "system/file.h"
# include "system/mutex.h"
# define KEY_SIZE 2 // bytes
# define ffdb_no(__p) \
		(__p)->no
// bind rivet to pile or record
# define ffdb_bind(__p, __no) \
		(__p)->no = __no
# define FFDBF_MAG0 'f'
# define FFDBF_MAG1 'f'
# define FFDBF_MAG2 'd'
# define FFDBF_MAG3 'b'
// needs testing

/*
	TODO:
		remove message and use microcode as its easer

*/

/*
	TODO
	ffdb_off_t - not ff_uint_t
*/

# define ffdb_hdrsize sizeof(struct ffdb_hdr)
# define FFDB_NULL ((ff_uint_t)~0)
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
	_ff_db_msg_del_record,
	_ff_db_msg_write,
	_ff_db_msg_read,
	_ff_db_msg_record_alloc,
	_ff_db_msg_record_free,
	_ff_db_msg_rivet,
	_ff_db_msg_derivet,
	_ff_db_msg_rivetto,
	_ff_db_msg_bind,
	_ff_db_msg_acquire_slot,
	_ff_db_msg_scrap_slot,
	_ff_db_msg_exist,
	_ff_db_msg_recstat
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

typedef ff_u8_t ff_db_key[KEY_SIZE];

typedef ff_u8_t ff_db_err;
# define MAX_PILES 20
/*
	need to rething
	'struct ff_db_msg'
*/
struct ff_db_msg {
	ff_u8_t kind;	 
};

struct ff_db_rep {
	ff_u8_t type;
};

struct ffdb_record_hdr {
	ff_u32_t p;
	ff_uint_t size;
	ff_uint_t no;
};

struct ffdb_pile_hdr {
	ff_u16_t nr;
	ff_u32_t rec;
	ff_uint_t no;
	ff_u32_t next;
};

typedef struct ff_db_tape {
	void *text;
	ff_uint_t len;
} *ff_db_tapep;

struct ffdb_hdr {
	char ident[4];
	ff_u32_t top, off;
	ff_u32_t bin;

	ff_u32_t pile;

	ff_u16_t fresh;

	ff_uint_t nd;
	ff_u16_t diched;
};

/*
	hold the underworld and surface together
*/
typedef struct ffdb_rivet {
	struct ffdb_rivet *fd, **bk;
	struct ffdb_rivet *next;
	ff_u16_t no;
	void *to;
} *ffdb_rivetp;

struct ffdb_pile;
struct ffdb_record;

struct ffdb {
	FF_FILE *file;
	ff_u32_t off;
	ff_mlock_t lock;
	struct ffdb_pile *top, *free[20], **next;
	struct ffly_map map;
};

typedef struct ff_db_user {
	ff_u8_t const *id;
	ff_u32_t passkey;
	ff_i8_t loggedin;
	ff_u8_t auth_level;
	ff_u64_t enckey;
} *ff_db_userp;

/*
	do we need this ???
*/
typedef struct ff_dbd {
	void **list;
	struct ffly_map users; 
	struct ffdb db;
} *ff_dbdp;

typedef struct ffdb_pile {
	struct ffly_map map; 
	struct ffdb_record *top;
	struct ffdb_pile *prev, *next;
	ff_mlock_t lock;
	ff_u16_t no;
} *ffdb_pilep;

typedef struct ffdb_record {
	ff_u32_t p;
	struct ffdb_record *prev, *next;
	ff_uint_t size;
	ff_u16_t no;
} *ffdb_recordp;

typedef struct ffdb_recstat {
	ff_uint_t size;
} *ffdb_recstatp;

typedef struct ffdb_blkd* ffdb_blkdp;

typedef struct ffdb* ffdbp;
typedef struct ff_db_msg* ff_db_msgp;

void ff_db_keygen(ff_u8_t*);
void ff_db_add_key(ff_dbdp, ff_u8_t*);
void ff_db_rm_key(ff_dbdp, ff_u8_t*);
ff_u8_t ff_db_valid_key(ff_dbdp, ff_u8_t*);

ff_db_userp ff_db_get_user(ff_dbdp, ff_u8_t const*, ff_uint_t, ff_err_t*);
ff_db_userp ff_db_add_user(ff_dbdp, ff_u8_t const*, ff_uint_t, ff_u32_t);
void ff_db_del_user(ff_dbdp, ff_u8_t const*, ff_uint_t, ff_u32_t);

ff_err_t ff_db_snd_key(FF_SOCKET*, ff_u8_t*, ff_u64_t);
ff_err_t ff_db_rcv_key(FF_SOCKET*, ff_u8_t*, ff_u64_t);

char const* ff_db_errst(ff_db_err);
ff_err_t ff_db_snd_err(FF_SOCKET*, ff_err_t);
ff_err_t ff_db_rcv_err(FF_SOCKET*, ff_err_t*);

ff_err_t ff_db_snd_errno(FF_SOCKET*, ff_db_err);
ff_err_t ff_db_rcv_errno(FF_SOCKET*, ff_db_err*);
ff_err_t ff_db_sndmsg(FF_SOCKET*, ff_db_msgp);
ff_err_t ff_db_rcvmsg(FF_SOCKET*, ff_db_msgp);
ffdb_recordp ffdb_record_creat(ffdbp, ffdb_pilep, ff_uint_t);
ffdb_recordp ffdb_record_fetch(ffdbp, char const*, char const*);
void ffdb_record_del(ffdbp, ffdb_pilep, ffdb_recordp);
void ffdb_record_alloc(ffdbp, ffdb_recordp);
void ffdb_record_free(ffdbp, ffdb_recordp);
void ffdb_record_stat(ffdbp, ffdb_recordp, ffdb_recstatp);
ff_i8_t ffdb_exist(ff_u16_t);

void ffdb_write(ffdbp, ffdb_pilep, ffdb_recordp, ff_off_t, void*, ff_uint_t);
void ffdb_read(ffdbp, ffdb_pilep, ffdb_recordp, ff_off_t, void*, ff_uint_t);
void ffdb_rivet(ff_u16_t, void*);
void ffdb_derivet(ff_u16_t);
void* ffdb_rivetto(ff_u16_t);
ff_u16_t ffdb_obtain_no();
void ffdb_ditch_no(ff_u16_t); 

void ffdb_settle(ffdbp);

ffdb_pilep ffdb_pile_creat(ffdbp);
void ffdb_record_alias(ffdbp, ffdb_pilep, char const*, ffdb_recordp); 
void ffdb_pile_alias(ffdbp, char const*, ffdb_pilep);
ffdb_pilep ffdb_pile_fetch(ffdbp, char const*);
void ffdb_pile_del(ffdbp, ffdb_pilep);

ff_uint_t ffdb_alloc(ffdbp, ff_uint_t);
void ffdb_free(ffdbp, ff_uint_t);
ff_err_t ffdb_init(ffdbp);
ff_err_t ffdb_cleanup(ffdbp);
ff_err_t ffdb_open(ffdbp, char const*);
ff_err_t ffdb_close(ffdbp);

// db/store.c
void ffdb_settle(ffdbp);
// db/load.c
void ffdb_load(ffdbp);
extern ff_u16_t ffdb_ndiched[100];
extern ff_u16_t *ffdb_nnext;
extern ff_u16_t ffdb_nfresh;
# endif /*__ffly__db__h*/
