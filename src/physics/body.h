# ifndef __ffly__physics__body__h
# define __ffly__physics__body__h
# include "../ffint.h"
# include "../puppet.h"

typedef struct ffly_phys_body {
	ffly_puppetp puppet;
	
	ff_uint_t mass;
	ff_uint_t velocity;
	ff_u8_t dir;
} *ffly_phys_bodyp;

ff_uint_t ffly_physical_body(ffly_puppetp);
void ffly_physical_body_update(ff_uint_t);
void ffly_set_direction(ff_uint_t, ff_u8_t);
void ffly_set_velocity(ff_uint_t, ff_uint_t);
# endif /*__ffly__physics__body__h*/
