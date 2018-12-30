# ifndef __ffly__physics__body__h
# define __ffly__physics__body__h
# include "../ffint.h"
# include "../polygon.h"
# include "../model.h"
# include "../system/mutex.h"
# include "../types.h"
/*
	move out of phy* directory

	legal velocity values: for now
	0.0-0.9
*/

# define ffly_body_vertex(__body, __x, __y, __z) \
	ffly_vertex3((__body)->model->poly+(__body)->model->off, __x, __y, __z)

#define GRAVITY_ENABLE 0x01

typedef struct ffly_uni* ffly_unip;
typedef struct ffly_lot* ffly_lotp;
/*
	might move this out of /physics
*/
typedef struct ffly_pnode {
	ff_int_t x, y, z;
} *ffly_pnodep;

typedef struct ffly_phy_body {
	ffly_modelp model;

	// remove
	ff_uint_t xl, yl, zl;
	// for now just int
	ff_uint_t mass;

	ff_u8_t velocity;
	void(*get)(ff_u8_t, long long, void*);
	void *arg;
	/*
		idk
	*/
	ff_uint_t *x, *y, *z;

/*
	should rename??? remove velocity part?
*/
	float angular_velocity;
	float gravity;

	// help locate what lot this body is in
	ffly_lotp lot;
	struct ffly_phy_body ***p;
	ffly_pnodep nodes;
	ff_uint_t nn;
	// direction
	ff_u8_t dir;
	ff_mlock_t lock;
	ff_u8_t flags;

	struct ffly_phy_body **bk, *next;
	ff_u32_t id;
} *ffly_phy_bodyp;

typedef ffly_phy_bodyp* ffly_phy_bodypp;
typedef ffly_phy_bodypp* ffly_phy_bodyppp;

void ffly_pnode(ffly_phy_bodyp, ff_int_t, ff_int_t, ff_int_t);
ffly_phy_bodyp ffly_phy_body_top();
void ffly_phy_body_fd(ffly_phy_bodyp*);

ffly_phy_bodyp ffly_get_phy_body(ff_u32_t);
ff_u32_t ffly_physical_body(void(*)(ff_u8_t, long long, void*), ff_u8_t, void*);
void ffly_body_cleanup();
void ffly_physical_body_update(ffly_unip, ff_uint_t, ff_u32_t);
void ffly_set_direction(ff_u32_t, ff_u8_t);
void ffly_set_velocity(ff_u32_t, float);
void ffly_set_mass(ff_u32_t, ff_uint_t);
void ffly_set_angular_velocity(ff_u32_t, float);
# endif /*__ffly__physics__body__h*/
