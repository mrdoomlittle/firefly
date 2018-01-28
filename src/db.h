# ifndef __ffly__db__h
# define __ffly__db__h
# include <mdlint.h>
# include "net.h"
# include "types/err_t.h"
# include "types/off_t.h"
# include "system/map.h"
# include "system/file.h"
enum {
    _ffdb_msg_login,
    _ffdb_msg_logout,
    _ffdb_msg_pulse,
    _ffdb_msg_shutdown,
    _ffdb_msg_disconnect
};

# define MAX_PILES 20
struct ffdb_blkd {
    mdl_uint_t size, end, off;
    struct ffdb_blkd *fd, *bk, *p;
    mdl_u64_t prev, next;
    mdl_u8_t inuse;
};

struct ffdb_msg {
    mdl_u8_t kind : 5;
    mdl_u8_t err : 1;   
};

struct ffdb_pile;
struct ffdb_record;

struct ffdb {
    FF_FILE *file;
    ffly_off_t off;
    
    struct ffdb_pile *top, *free[20], **next;
    struct ffly_map map;
};

struct ffdb_pile {
    struct ffly_map map; 
    struct ffdb_record *top;
    struct ffdb_pile *prev, *next;
};

struct ffdb_record {
    mdl_uint_t p;
    struct ffdb_record *prev, *next;
};

typedef struct ffdb_blkd* ffdb_blkdp;
typedef struct ffdb_record* ffdb_recordp;
typedef struct ffdb_pile* ffdb_pilep;
typedef struct ffdb* ffdbp;
typedef struct ffdb_msg* ffdb_msgp;

ffly_err_t ffdb_sndmsg(FF_SOCKET*, ffdb_msgp);
ffly_err_t ffdb_rcvmsg(FF_SOCKET*, ffdb_msgp);
ffdb_recordp ffdb_creat_record(ffdbp, ffdb_pilep, mdl_uint_t);
ffdb_recordp ffdb_fetch_record(ffdbp, char const*, char const*);
void ffdb_del_record(ffdbp, ffdb_pilep, ffdb_recordp);
void ffdb_write(ffdbp, ffdb_pilep, ffdb_recordp, ffly_off_t, void*, mdl_uint_t);
void ffdb_read(ffdbp, ffdb_pilep, ffdb_recordp, ffly_off_t, void*, mdl_uint_t);

ffdb_pilep ffdb_creat_pile(ffdbp);
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
