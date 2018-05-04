# ifndef __ffly__gravity__h
# define __ffly__gravity__h
# include "ffint.h"
# include "physics/body.h"
typedef struct ffly_uni* ffly_unip;

# define GRAVITY_CONST 0.01

void ffly_gravity_apply(ffly_unip, ffly_phy_bodyp, ff_uint_t);
void ffly_gravity_init(ff_uint_t, ff_uint_t, ff_uint_t);
void ffly_gravity_add(float, ff_uint_t, ff_uint_t, ff_uint_t);
void ffly_gravity_sub(float, ff_uint_t, ff_uint_t, ff_uint_t);
float ffly_gravity_at(ff_uint_t, ff_uint_t, ff_uint_t);
void ffly_gravity_cleanup();
# endif /*__ffly_gravity__h*/
