# ifndef __ffly__bh__h
# define __ffly__bh__h
# include "ffint.h"
# include "types.h"
# include "net.h"
// brick stash
/*
	might rename
	rundown:
		download/upload centre for bricks
		bricks can be stored and retrieved.
*/

enum {
	// new brick
	_bhop_bnew,
	// get rid of brick
	_bhop_brid,
	_bhop_bopen,
	_bhop_bwrite,
	_bhop_bread,
	_bhop_bclose,
	_bhop_disconnect
};

struct bhop {
	ff_u8_t kind;

};

typedef struct ff_bh {
	FF_SOCKET *sock;
} *ff_bhp;

extern struct ff_bh bh;

void ff_bh_open(ff_bhp);
void ff_bh_connect(ff_bhp, char const*, ff_u16_t);
void ff_bh_disconnect(ff_bhp);
void ff_bh_close(ff_bhp);

void ff_bhs_prep(void);
void ff_bhs_open(void);
void ff_bhs_start(void);
void ff_bhs_closedown(void);
void ff_bhs_cleanup(void);

ff_u32_t ff_bh_bnew(ff_bhp, ff_u8_t, ff_err_t*);
ff_err_t ff_bh_brid(ff_bhp, ff_u32_t);
ff_err_t ff_bh_bopen(ff_bhp, ff_u32_t);
ff_err_t ff_bh_bwrite(ff_bhp, ff_u32_t, void*, ff_uint_t, ff_uint_t);
ff_err_t ff_bh_bread(ff_bhp, ff_u32_t, void*, ff_uint_t, ff_uint_t);
ff_err_t ff_bh_bclose(ff_bhp, ff_u32_t);

ff_err_t ff_bh_oprcv(FF_SOCKET*, struct bhop*);
ff_err_t ff_bh_opsnd(FF_SOCKET*, struct bhop*);
# endif /*__ffly__bh__h*/
