# ifndef __ffly__bond__frag__h
# define __ffly__bond__frag__h
# include "../ffint.h"
struct frag {
	ff_uint_t size;
	ff_u8_t data[24];
	ff_u8_t x_len;
	ff_u32_t adr;
	ff_uint_t f;
	ff_int_t m, bs;
	struct frag *next;
};

struct frag extern *fr_head;
struct frag *bond_fbn(ff_uint_t);
struct frag* bond_fnew(ff_u64_t, ff_uint_t, ff_uint_t);
void bond_fdestroy(struct frag*);
# endif /*__ffly__bond__frag__h*/
