# include "contact.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "../system/sched.h"
# include "../system/mutex.h"
typedef struct sentinel {
	ffly_phy_bodyp a, b;
	ff_i8_t(*alert)(ff_u8_t, ff_u8_t, ffly_phy_bodyp, ffly_phy_bodyp);
	ff_u32_t sched_id;
	ff_uint_t off;
} *sentinelp;

ff_mlock_t static lock = FFLY_MUTEX_INIT;
static sentinelp *sentinels = NULL;
static ff_uint_t off = 0;

# define MITS 3

ff_i8_t static
tick(void *__arg) {
	sentinelp se = (sentinelp)__arg;
	ffly_phy_bodyp a = se->a;
	ffly_phy_bodyp b = se->b;

	ffly_mutex_lock(&a->lock);	
	ffly_mutex_lock(&b->lock);

	ffly_pnodep cur;

	cur = a->nodes;
	ffly_pnodep end;

	end = cur+a->nn;

	ffly_pnodep a0, b0;
	while(cur < end) {
		a0 = cur++;
		b0 = cur;

		ff_uint_t i;

		i = 0;
		while(i != b->nn) {
			ffly_pnodep n;
			n = b->nodes+i;

			ff_int_t v;

			ff_int_t ax, ay;
			ff_int_t bx, by;

			ax = *a->x;
			ay = *a->y;

			bx = *b->x;
			by = *b->y;

			ff_int_t x,y;
			x = bx+n->x;
			y = by+n->y;

			v = (((x-(ax+a0->x))*((ay+b0->y)-y))-(((ax+b0->x)-x)*(y-(ay+a0->y))));
			if (v >= -2 && v <= 2) {
				se->alert(a0-a->nodes, i, a, b);		
			}

			
			i++;
		}
	}

	ffly_mutex_unlock(&a->lock);
	ffly_mutex_unlock(&b->lock);
	return -1;
/*
	ff_uint_t a_x = *a->x;
	ff_uint_t a_y = *a->y;
	ff_uint_t a_z = *a->z;

	ff_uint_t b_x = *b->x;
	ff_uint_t b_y = *b->y;
	ff_uint_t b_z = *b->z;

	ff_uint_t a_xe = a_x+a->xl;
	ff_uint_t a_ye = a_y+a->yl;
	ff_uint_t a_ze = a_z+a->zl;

	ff_uint_t b_xe = b_x+b->xl;
	ff_uint_t b_ye = b_y+b->yl;
	ff_uint_t b_ze = b_z+b->zl;

	// simple

	ff_u8_t c2 = (a_xe>=b_x && a_xe<=b_xe) && (a_ye>=b_y && a_ye<b_y+MITS);
	ff_u8_t c1 = (a_xe>=b_x && a_xe<b_x+MITS) && (a_y>=b_y && a_y<b_ye);

	ff_u8_t c0 = (b_xe>=a_x && b_xe<=a_xe) && (b_ye>=a_y && b_ye<a_y+MITS);
	ff_u8_t c3 = (b_xe>=a_x && b_xe<a_x+MITS) && (b_y>=a_y && b_y<a_ye);

	ff_u8_t from;
	// top
	if (c0) {
		from = _ffly_contact_c0;
		goto _alert;
	}

	// right
	if (c1) {
		from = _ffly_contact_c1;
		goto _alert;
	}
	
	// bottom
	if (c2) {
		from = _ffly_contact_c2;
		goto _alert;
	}

	// left
	if (c3) {
		from = _ffly_contact_c3;
		goto _alert;
	}

	goto _sk;
_alert:
	se->alert(from, a, b);
_sk:
	return -1;
*/
}

ff_uint_t
ffly_phy_contact(ffly_phy_bodyp __a, ffly_phy_bodyp __b, ff_i8_t(*__alert)(ff_u8_t, ff_u8_t, ffly_phy_bodyp, ffly_phy_bodyp)) {
	ffly_mutex_lock(&lock);
	if (!sentinels) {
		sentinels = (sentinelp*)__ffly_mem_alloc(sizeof(sentinelp));
		off++;
	} else
		sentinels = (sentinelp*)__ffly_mem_realloc(sentinels, (++off)*sizeof(sentinelp));
	ffly_mutex_unlock(&lock);

	sentinelp se = (*(sentinels+(off-1)) = __ffly_mem_alloc(sizeof(struct sentinel)));
	se->off = off-1;
	se->a = __a;
	se->b = __b;
	se->alert = __alert;
	se->sched_id = ffly_schedule(tick, se, 10);
}

void ffly_sentinel_cleanup(void) {
	sentinelp *p = sentinels;
	sentinelp *end = p+off;
	while(p != end) {
		ffly_sched_rm((*p)->sched_id);
		__ffly_mem_free(*p);
		p++;
	}

	if (sentinels != NULL)
		__ffly_mem_free(sentinels);
}
