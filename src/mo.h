# ifndef __ffly__mo__h
# define __ffly__mo__h
# include "ffint.h"
/*
	TODO:
		rename
		- graphics memory object
	NOTE: 
		default memory will be used for now
*/


typedef struct ffly_mo {
	
	void *map;
} *ffly_mop;

ffly_mop ffly_mo_new(void);
void ffly_mo_destroy(ffly_mop);
void ffly_mo_map(ffly_mop, ff_uint_t);
void ffly_mo_unmap(ffly_mop);

ffly_mop ffly_mo_read(ffly_mop, void*, ff_uint_t, ff_u32_t);
ffly_mop ffly_mo_write(ffly_mop, void*, ff_uint_t, ff_u32_t);
# endif /*__ffly__mo__h*/
