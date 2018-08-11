# include "body.h"
# include "../maths/round.h"
# include "clock.h"
# include "../maths/abs.h"
# include "../system/io.h"
# include "../gravity.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "../system/error.h"
# define PAGE_SHIFT 4
# define PAGE_SIZE (1<<PAGE_SHIFT)

ff_err_t ffly_uni_attach_body(ffly_unip, ffly_phy_bodyp);
ff_err_t ffly_uni_detach_body(ffly_unip, ffly_phy_bodyp);

/*				page offset				page no.
		|							|				|
	ID:   0000 0000 0000 0000 0000    0000 0000 0000
*/

// change this
static ffly_phy_bodyp *bodies = NULL;
ff_u64_t static off = 0;
ff_uint_t static page_c = 0;

static ffly_phy_bodyp top = NULL;
# define get_body(__id) \
	((*(bodies+(__id&0xfff)))+((__id>>12)&0xfffff))

ffly_phy_bodyp ffly_get_phy_body(ff_u32_t __id) {
	return get_body(__id);
}

ffly_phy_bodyp ffly_phy_body_top() {
	return top;
}

void ffly_phy_body_fd(ffly_phy_bodyp *__p) {
	*__p = (*__p)->next;
}

ff_u32_t ffly_physical_body(ff_uint_t *__x, ff_uint_t *__y, ff_uint_t *__z, ff_u8_t __flags) {
	ffly_phy_bodyp body;

	ff_uint_t page = off>>PAGE_SHIFT;
	ff_uint_t pg_off;
	if (!bodies) {
		bodies = (ffly_phy_bodyp*)__ffly_mem_alloc(sizeof(ffly_phy_bodyp));	
		page_c++;
	} else {
		if (page>page_c-1)
			bodies = (ffly_phy_bodyp*)__ffly_mem_realloc(bodies, (++page_c)*sizeof(ffly_phy_bodyp));
		else
			goto _sk;
	}

	*(bodies+page) = (ffly_phy_bodyp)__ffly_mem_alloc(PAGE_SIZE*sizeof(struct ffly_phy_body));
_sk:
	pg_off = (off++)-(page*PAGE_SIZE);
	body = (*(bodies+page))+pg_off;
	if (top != NULL)
		top->bk = &body->next;
	body->next = top;
	body->bk = &top;
	top = body;

	body->velocity = 0;
	body->angular_velocity = 0.0;
	body->gravity = 0.0;
	body->dir = 26;
	body->x = __x;
	body->y = __y;
	body->z = __z;
	body->nodes = NULL;
	body->nn = 0;
	body->mass = 0;
	body->lot = NULL;
	body->lock = FFLY_MUTEX_INIT;
	body->flags = __flags;
	return (body->id = (page&0xfff)|((pg_off&0xfffff)<<12));
}

/*
	there are 26 diffrent direction that we can take,
	so we are going to use this as it would be slower if we used c function,
	as gcc sets up the stack for it and its extra instructions that we dont need,
	and thus more time being taken up.
*/

// not all directions
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

/*
	mid section: 1x1x1
	|----|	|----|	|----|
	|    |	| a0 |	|    |
	|----|	|----|	|----|

	|----|			|----|
	| a3 |			| a1 |
	|----|			|----|

	|----|	|----|	|----|
	|    |	| a2 |	|    |
	|----|	|----|	|----|


	front: 0x0x0
	|----|	|----|	|----|
	|    |	|    |	|    |
	|----|	|----|	|----|

	|----|	|----|	|----|
	|    |	| a5 |	|    |
	|----|	|----|	|----|

	|----|	|----|	|----|
	|    |	|    |	|    |
	|----|	|----|	|----|


	back: 2x2x2
	|----|	|----|	|----|
	|    |	|    |	|    |
	|----|	|----|	|----|

	|----|	|----|	|----|
	|    |	| a4 |	|    |
	|----|	|----|	|----|

	|----|	|----|	|----|
	|    |	|    |	|    |
	|----|	|----|	|----|
*/

void static
move(ffly_phy_bodyp __body, ff_uint_t __delta) {
	ff_uint_t velocity = __body->velocity;
	ff_uint_t *x = __body->x;
	ff_uint_t *y = __body->y;
	ff_uint_t *z = __body->z;

	ff_uint_t ang = (ff_uint_t)ffly_round(((((float)__delta)*0.2)*(__body->angular_velocity/PHY_TIME_PERIOD))*10.0);

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

void ffly_pnode(ffly_phy_bodyp __body, ff_int_t __x, ff_int_t __y, ff_int_t __z) {
	ffly_mutex_lock(&__body->lock);
	if (!__body->nodes) {
		__body->nodes = __ffly_mem_alloc(sizeof(struct ffly_pnode));
		__body->nn++;
	} else {
		__body->nodes = __ffly_mem_realloc(__body->nodes, (++__body->nn)*sizeof(struct ffly_pnode));
	}

	ffly_pnodep n;
	
	n = __body->nodes+(__body->nn-1);
	n->x = __x;
	n->y = __y;
	n->z = __z;
	ffly_mutex_unlock(&__body->lock);
}

void ffly_physical_body_update(ffly_unip __uni, ff_uint_t __delta, ff_u32_t __id) {
	ffly_phy_bodyp body = get_body(__id);
	ff_uint_t *x = body->x;
	ff_uint_t *y = body->y;
	ff_uint_t *z = body->z;
	ff_u8_t grav;

	grav = (body->flags&GRAVITY_ENABLE)^GRAVITY_ENABLE;

	if (_err(ffly_uni_detach_body(__uni, body))) {
		ffly_printf("failed to detach body.\n");
	}
	if (!grav)
		ffly_gravity_sub(body->gravity, *x, *y, *z);

	move(body, __delta);

	if (!grav) {
		body->gravity = ((float)body->mass)*0.01;

		ffly_gravity_add(body->gravity, *x, *y, *z);
	}
	if (_err(ffly_uni_attach_body(__uni, body))) {
		ffly_printf("failed to attach body.\n");
	}
}

void ffly_body_cleanup() {
	if (bodies != NULL) {
		ffly_phy_bodyp *page = bodies;
		while(page != bodies+page_c) {
			__ffly_mem_free(*page);
			page++;
		}
		__ffly_mem_free(bodies);
	}
}

void ffly_set_direction(ff_u32_t __id, ff_u8_t __dir) {
	get_body(__id)->dir = __dir;
}

void ffly_set_velocity(ff_u32_t __id, float __velocity) {
	get_body(__id)->velocity = __velocity*10;
}

void ffly_set_mass(ff_u32_t __id, ff_uint_t __mass) { 
	get_body(__id)->mass = __mass;
}

void ffly_set_angular_velocity(ff_u32_t __id, float __velocity) {
	get_body(__id)->angular_velocity = __velocity;
}
