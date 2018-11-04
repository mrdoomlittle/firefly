# ifndef __ffly__bh__h
# define __ffly__bh__h
# include "ffint.h"
# include "types.h"
# include "net.h"
/*
	rundown:
		download/upload centre for bricks
		bricks can be stored and retrieved.

	TODO:
		remove bhop and replace with microcode
		as its easer to manage at a lower level

		at the moment its free for all
		- FIX
*/

#define _bhop_bnewm			0x00
#define _bhop_bridm			0x01
#define _bhop_bnew			0x02
#define _bhop_brid			0x03
#define _bhop_bopen			0x04
#define _bhop_bwrite		0x05
#define _bhop_bread			0x06
#define _bhop_bclose		0x07
#define _bhop_disconnect	0x08
#define _bhop_bexist		0x09
typedef struct bh_tape {
	void *text;
	ff_uint_t len;
} *bh_tapep;

bh_tapep bh_tape_new(ff_uint_t);
void bh_tape_destroy(bh_tapep);

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

ff_err_t ff_bh_bnewm(ff_bhp, ff_u8_t, ff_u32_t*, ff_uint_t);
ff_err_t ff_bh_bridm(ff_bhp, ff_u32_t*, ff_uint_t);
ff_u32_t ff_bh_bnew(ff_bhp, ff_u8_t, ff_err_t*);
ff_err_t ff_bh_brid(ff_bhp, ff_u32_t);
ff_err_t ff_bh_bopen(ff_bhp, ff_u32_t);
ff_err_t ff_bh_bwrite(ff_bhp, ff_u32_t, void*, ff_uint_t, ff_uint_t);
ff_err_t ff_bh_bread(ff_bhp, ff_u32_t, void*, ff_uint_t, ff_uint_t);
ff_err_t ff_bh_bclose(ff_bhp, ff_u32_t);
# endif /*__ffly__bh__h*/
