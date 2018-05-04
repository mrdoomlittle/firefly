# include "body.h"
# include "../maths/round.h"
# include "clock.h"
# include "../maths/abs.h"
# include "../system/io.h"
# include "../gravity.h"
ff_err_t ffly_uni_attach_body(ffly_unip, ffly_phy_bodyp);
ff_err_t ffly_uni_detach_body(ffly_unip, ffly_phy_bodyp);

// change this
static struct ffly_phy_body bodies[20];
ffly_phy_bodyp static fresh = bodies;

# define get_body(__id) \
	(bodies+__id)

ffly_phy_bodyp ffly_get_phy_body(ff_uint_t __id) {
	return get_body(__id);
}

ffly_phy_bodyp ffly_phy_body_top() {
	return bodies;
}

ffly_phy_bodyp ffly_phy_body_end() {
	return fresh;
}

ff_uint_t ffly_physical_body(ff_uint_t *__x, ff_uint_t *__y, ff_uint_t *__z) {
	ffly_phy_bodyp body = fresh;
	body->velocity = 0;
	body->gravity = 0.0;
	body->dir = 26;
	body->x = __x;
	body->y = __y;
	body->z = __z;
	body->lot = NULL;
	return (fresh++)-bodies;
}

/*
	there are 26 diffrent direction that we can take,
	so we are going to use this as it would be slower if we used c function,
	as gcc sets up the stack for it and its extra instructions that we dont need,
	and thus more time being taken up.
*/

void _move_a0();
void _move_a1();
void _move_a2();
void _move_a3();
void _move_a4();
void _move_a5();

void *dir[] = {
	_move_a0,
	_move_a1,
	_move_a2,
	_move_a3,
	_move_a4,
	_move_a5
};

void static
move(ffly_phy_bodyp __body, ff_uint_t __delta) {
	ff_uint_t velocity = __body->velocity;

	ff_uint_t *x = __body->x;
	ff_uint_t *y = __body->y;
	ff_uint_t *z = __body->z;

	ff_uint_t ang = (ff_uint_t)ffly_round(((((float)__delta)*0.2)*(__body->angle/TIME_PERIOD))*10.0);

	if (__body->dir == 26) return;
	__asm__("jmp *%0" : : "r"(dir[__body->dir]));

	__asm__("_move_a0:\n\t");
	*y-=velocity;
	*x = *x+ang;

	__asm__("jmp _end");
	__asm__("_move_a1:\n\t");
	*x+=velocity;
	*y = *y+ang;

	__asm__("jmp _end");
	__asm__("_move_a2:\n\t");
	*y+=velocity;
	*x = *x-ang;

	__asm__("jmp _end");
	__asm__("_move_a3:\n\t");
	*x-=velocity;
	*y = *y-ang;

	__asm__("jmp _end");
	__asm__("_move_a4:\n\t");
	*z+=velocity;

	__asm__("jmp _end");
	__asm__("_move_a5:\n\t");
	*z-=velocity;

	__asm__("_end:\n\t");
}

void ffly_physical_body_update(ffly_unip __uni, ff_uint_t __delta, ff_uint_t __id) {
	ffly_phy_bodyp body = get_body(__id);
	ff_uint_t *x = body->x;
	ff_uint_t *y = body->y;
	ff_uint_t *z = body->z;
	ffly_uni_detach_body(__uni, body);
	ffly_gravity_sub(body->gravity, *x, *y, *z);

	move(body, __delta);
	body->gravity = ((float)body->mass)*0.04;

	ffly_gravity_add(body->gravity, *x, *y, *z);
	ffly_uni_attach_body(__uni, body);
}

void ffly_set_direction(ff_uint_t __id, ff_u8_t __dir) {
	get_body(__id)->dir = __dir;
}

void ffly_set_velocity(ff_uint_t __id, float __velocity) {
	get_body(__id)->velocity = __velocity*10;
}

void ffly_set_mass(ff_uint_t __id, ff_uint_t __mass) { 
	get_body(__id)->mass = __mass;
}

void ffly_set_angle(ff_uint_t __id, float __angle) {
	get_body(__id)->angle = __angle;
}
