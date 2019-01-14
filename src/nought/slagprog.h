# ifndef __ffly__slagprog__h
# define __ffly__slagprog__h
# include "../ffint.h"
#define nt_slg_st 0x00
#define nt_slg_ld 0x01

/*
	ignore name i dont know enough to call it a fragment
*/
struct slgio {
	ff_u32_t px;
	ff_u16_t z;
};


extern struct slgio slg;
void nt_slg_prog(void);
void nt_slg_run(ff_u8_t*, ff_uint_t);
void nt_slags_exec(void);
# endif /*__ffly__slagprog__h*/
