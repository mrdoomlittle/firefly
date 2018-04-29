# include "body.h"
static struct ffly_phys_body bodies[20];
ffly_phys_bodyp static fresh = bodies;

# define get_body(__id) \
	(bodies+__id)

ff_uint_t ffly_physical_body(ffly_puppetp __puppet) {
	ffly_phys_bodyp body = fresh;
	body->velocity = 0;
	body->dir = 26;
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
	ffly_puppetp puppet = __body->puppet;
	if (__body->dir == 26) return;
	__asm__("jmp *%0" : : "r"(dir[__body->dir]));

	__asm__("_move_a0:\n\t");
	(*puppet->y)-=velocity;

	__asm__("jmp _end\n\t");
	__asm__("_move_a1:\n\t");
	(*puppet->x)+=velocity;
	
	__asm__("jmp _end\n\t");
	__asm__("_move_a2:\n\t");
	(*puppet->y)+=velocity;

	__asm__("jmp _end\n\t");
	__asm__("_move_a3:\n\t");
	(*puppet->x)-=velocity;

	__asm__("jmp _end\n\t");
	__asm__("_move_a4:\n\t");
	(*puppet->z)+=velocity;

	__asm__("jmp _end\n\t");
	__asm__("_move_a5:\n\t");
	(*puppet->z)-=velocity;

	__asm__("_end:\n\t");
}

void ffly_physical_body_update(ff_uint_t __id) {
	ffly_phys_bodyp body = get_body(__id);
	move(body);
}

void ffly_set_direction(ff_uint_t __id, ff_u8_t __dir) {
	get_body(__id)->dir = __dir;
}

void ffly_set_velocity(ff_uint_t __id, ff_uint_t __velocity) {
	get_body(__id)->velocity = __velocity;
}
