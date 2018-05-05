# ifndef __ffly__physics__body__h
# define __ffly__physics__body__h
# include "../ffint.h"
# include "../polygon.h"
/*
	legal velocity values: for now
	0.0-0.9
*/

# define ffly_body_vertex(__body, __x, __y, __z) \
	ffly_vertex3(&(__body)->shape, __x, __y, __z)

typedef struct ffly_uni* ffly_unip;
typedef struct ffly_lot* ffly_lotp;
/*
	might move this out of /physics
*/

typedef struct ffly_phy_body {
	ffly_polygon shape;
	ff_u8_t *texture;
	ff_uint_t xl, yl, zl;
	// for now just int
	ff_uint_t mass;

	ff_u8_t velocity;
	ff_uint_t *x, *y, *z;
	float angular_velocity;
	float gravity;

	// help locate what lot this body is in
	ffly_lotp lot;

	// direction
	ff_u8_t dir;

	struct ffly_phy_body *prev, *next;
} *ffly_phy_bodyp;

typedef ffly_phy_bodyp* ffly_phy_bodypp;
typedef ffly_phy_bodypp* ffly_phy_bodyppp;

ffly_phy_bodyp ffly_phy_body_top();
void ffly_phy_body_fd(ffly_phy_bodyp*);
ffly_phy_bodyp ffly_phy_body_end();

ffly_phy_bodyp ffly_get_phy_body(ff_u32_t);
ff_u32_t ffly_physical_body(ff_uint_t*, ff_uint_t*, ff_uint_t*);
void ffly_body_cleanup();
void ffly_physical_body_update(ffly_unip, ff_uint_t, ff_u32_t);
void ffly_set_direction(ff_u32_t, ff_u8_t);
void ffly_set_velocity(ff_u32_t, float);
void ffly_set_mass(ff_u32_t, ff_uint_t);
void ffly_set_angular_velocity(ff_u32_t, float);
# endif /*__ffly__physics__body__h*/
