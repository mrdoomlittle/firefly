# ifndef __ffly__bond__frag__h
# define __ffly__bond__frag__h
# include "../ffint.h"
struct frag {
	ff_u64_t src;
	ff_uint_t size;
	ff_u8_t anx[24];
	ff_u8_t x_len;
};

struct frag* bond_fnew(ff_u64_t, ff_uint_t);
void bond_fdestroy(struct frag*);
# endif /*__ffly__bond__frag__h*/
