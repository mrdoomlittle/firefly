# ifndef __ffly__physics__body__h
# define __ffly__physics__body__h
# include "../ffint.h"
# include "../puppet.h"

/*
	legal velocity values: for now
	0.0-0.9
*/

typedef struct ffly_phys_body {
	ffly_puppetp puppet;
	
	// for now just int
	ff_uint_t mass;

	// store velocity as 8-bit number
	ff_u8_t velocity;
	ff_i8_t x, y, z;

	float angle;

	// direction
	ff_u8_t dir;
} *ffly_phys_bodyp;

ff_uint_t ffly_physical_body(ffly_puppetp);
void ffly_physical_body_update(ff_uint_t);
void ffly_set_direction(ff_uint_t, ff_u8_t);
void ffly_set_velocity(ff_uint_t, float);
void ffly_set_mass(ff_uint_t, ff_uint_t);
void ffly_set_angle(ff_uint_t, float);
# endif /*__ffly__physics__body__h*/
