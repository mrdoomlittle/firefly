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

#define _FF_S(__NAME) \
	_FF_ ## __NAME ## _S
#define _FF_LOGIN_S				8
#define _FF_LOGOUT_S			0
#define _FF_STORE_S				4
#define _FF_LOAD_S				4
#define _FF_RATIFYKEY_S			KEY_SIZE
#define _FF_SHUTDOWN_S			0	
#define _FF_PILE_CREAT_S		0
#define _FF_PILE_DEL_S			4
#define _FF_RECORD_CREAT_S		8
#define _FF_RECORD_DEL_S		8
#define _FF_WRITE_S				16
#define _FF_READ_S				16
#define _FF_RECORD_ALLOC_S		4
#define _FF_RECORD_FREE_S		4
#define _FF_RIVET_S				6
#define _FF_DERIVET_S			2
#define _FF_RIVETTO_S			6
#define _FF_BIND_S				7
#define _FF_ACQUIRE_SLOT_S		0
#define _FF_SCRAP_SLOT_S		4
#define _FF_EXIST_S				2
#define _FF_RECORD_STAT_S		4
// needs testing

/*
	TODO:
		remove message and use microcode as its easer
	
		rename to oyster well the replace db with oys


*/

/*
	TODO
	ffdb_off_t - not ff_uint_t
*/

#define _FFDB_REC_SIZE 0x00

# define ffdb_hdrsize sizeof(struct ffdb_hdr)
# define FFDB_NULL ((ff_uint_t)~0)
enum {
	_ff_db_auth_root,
	_ff_db_auth_user,
	_ff_db_auth_null
};

#define ff_db_rcv_error(__sock, __err, __err0) \
	ff_net_recv(__sock, __err, sizeof(ff_err_t), 0, __err);
#define ff_db_rcv_errno(__sock, __ern, __err) \
	ff_net_recv(__sock, __ern, sizeof(ff_db_err), 0, __err);
#define _ff_db_op_login 			0x00
#define _ff_db_op_logout			0x01
#define _ff_db_op_store				0x02
#define _ff_db_op_load				0x03
#define _ff_db_op_ratifykey			0x04
#define _ff_db_op_shutdown			0x05
#define _ff_db_op_pile_creat		0x06
#define _ff_db_op_pile_del			0x07
#define _ff_db_op_record_creat		0x08
#define _ff_db_op_record_del		0x09
#define _ff_db_op_write				0x0a
#define _ff_db_op_read				0x0b
#define _ff_db_op_record_alloc		0x0c
#define _ff_db_op_record_free		0x0d
#define _ff_db_op_rivet				0x0e
#define _ff_db_op_derivet			0x0f
#define _ff_db_op_rivetto			0x10
#define _ff_db_op_bind				0x11
#define _ff_db_op_acquire_slot		0x12
#define _ff_db_op_scrap_slot		0x13
#define _ff_db_op_exist				0x14
#define _ff_db_op_recstat			0x15
#define _ff_db_op_disconnect		0x16
#define _ff_db_op_req_errno			0x17
#define _ff_db_op_pulse				0x18
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

ff_db_tapep ff_db_tape_new(ff_uint_t);
void ff_db_tape_destroy(ff_db_tapep);

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

char const* ff_db_ostr(ff_u8_t);
void ff_db_keygen(ff_u8_t*);
void ff_db_add_key(ff_dbdp, ff_u8_t*);
void ff_db_rm_key(ff_dbdp, ff_u8_t*);
ff_u8_t ff_db_valid_key(ff_dbdp, ff_u8_t*);

ff_db_userp ff_db_get_user(ff_dbdp, ff_u8_t const*, ff_uint_t, ff_err_t*);
ff_db_userp ff_db_add_user(ff_dbdp, ff_u8_t const*, ff_uint_t, ff_u32_t);
void ff_db_del_user(ff_dbdp, ff_u8_t const*, ff_uint_t, ff_u32_t);

char const* ff_db_errst(ff_db_err);
ffdb_recordp ffdb_record_creat(ffdbp, ffdb_pilep, ff_uint_t);
ffdb_recordp ffdb_record_fetch(ffdbp, char const*, char const*);
void ffdb_record_del(ffdbp, ffdb_pilep, ffdb_recordp);
void ffdb_record_alloc(ffdbp, ffdb_recordp);
void ffdb_record_free(ffdbp, ffdb_recordp);
void ffdb_record_stat(ffdbp, ffdb_recordp, ffdb_recstatp);
void ff_db_record_info(ff_u8_t, long long, ffdb_recordp);
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
void ff_db_pile_info(ff_u8_t, long long, ffdb_pilep);
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
