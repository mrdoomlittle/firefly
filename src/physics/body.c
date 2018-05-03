# include "body.h"
# include "../maths/round.h"
# include "clock.h"
# include "../maths/abs.h"
# include "../system/io.h"
static struct ffly_phys_body bodies[20];
ffly_phys_bodyp static fresh = bodies;

# define get_body(__id) \
	(bodies+__id)

ff_uint_t ffly_physical_body(ffly_puppetp __puppet) {
	ffly_phys_bodyp body = fresh;
	body->velocity = 0;
	body->dir = 26;
	body->x = 0;
	body->y = 0;
	body->z = 0;
	body->puppet = __puppet;
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
move(ffly_phys_bodyp __body) {
	ff_uint_t velocity = __body->velocity;

	ff_i8_t *x = &__body->x;
	ff_i8_t *y = &__body->y;
	ff_i8_t *z = &__body->z;

	if (__body->dir == 26) return;
	__asm__("jmp *%0" : : "r"(dir[__body->dir]));

	__asm__("_move_a0:\n\t");
	*y-=velocity;
	*x = (ff_i8_t)(((float)*x)+ffly_round((__body->angle/TIME_PERIOD)*10.0));

	__asm__("jmp _end\n\t");
	__asm__("_move_a1:\n\t");
	*x+=velocity;
	*y = (ff_i8_t)(((float)*y)+ffly_round((__body->angle/TIME_PERIOD)*10.0));

	__asm__("jmp _end\n\t");
	__asm__("_move_a2:\n\t");
	*y+=velocity;
	*x = (ff_i8_t)(((float)*x)-ffly_round((__body->angle/TIME_PERIOD)*10.0));

	__asm__("jmp _end\n\t");
	__asm__("_move_a3:\n\t");
	*x-=velocity;
	*y = (ff_i8_t)(((float)*y)-ffly_round((__body->angle/TIME_PERIOD)*10.0));

	__asm__("jmp _end\n\t");
	__asm__("_move_a4:\n\t");
	*z+=velocity;

	__asm__("jmp _end\n\t");
	__asm__("_move_a5:\n\t");
	*z-=velocity;

	__asm__("_end:\n\t");
	if (*x >= 10) {
		(*__body->puppet->x)++;
		*x = 0;
	} else if (*x <= -10) {
		(*__body->puppet->x)--;
		*x = 0;
	}

	if (*y >= 10) {
		(*__body->puppet->y)++;
		*y = 0;
	 } else if (*y <= -10) {
		(*__body->puppet->y)--;
		*y = 0;
	}

	if (*z >= 10) {
		(*__body->puppet->z)++;
		*z = 0;
	} else if (*z <= -10) {
		(*__body->puppet->z)--;
		*z = 0;
	}
}

void ffly_physical_body_update(ff_uint_t __id) {
	ffly_phys_bodyp body = get_body(__id);
	move(body);
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
