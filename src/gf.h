# ifndef __ffly__gf__h
# define __ffly__gf__h
/*
	TODO:
		all graphics functions are to go thru here anything 'bron_...'()
		is to be gf.'...'()
*/
#include "graphics/struc.h"
struct ffly_gf {
	void(*tri2)(struct ffly_tri2*, ff_u16_t, ff_u32_t, ff_u32_t);
	void(*tri3)(struct ffly_tri3*, ff_u16_t, ff_u32_t, ff_u32_t, ff_u32_t);
};

#define gf_tri2	gf->tri2
#define gf_tri3	gf->tri3
extern struct ffly_gf *gf;
void ffly_gfload(void);
# endif /*__ffly__gf__h*/
