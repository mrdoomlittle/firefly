# include "gravity.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "physics/clock.h"
# include "maths/round.h"
# include "system/io.h"
# include "system/error.h"
# include "system/sched.h"
# include "clock.h"
# include "storage/reservoir.h"
# include "system/mutex.h"
# include "dep/mem_set.h"
/*
	taking each body and calculating the gravity on each one
	will take too long, so we are going this route gravity may
	be at a loss of quality but its faster and would take up less memory.
*/

ff_err_t ffly_uni_attach_body(ffly_unip, ffly_phy_bodyp);
ff_err_t ffly_uni_detach_body(ffly_unip, ffly_phy_bodyp);

/*
	0	1	2	3	4	5	6	- shift
	|	|	|	|	|	|	|
	1,	2,	4,	8,	16,	32,	64	- length (X*X)

	default: 16x16
*/

# define PAGE_DEATH 800//ms
# define PAGE_SHIFT 2
# define PAGE_SIZE (1<<PAGE_SHIFT)
# define ZONE_SHIFT 4

# define ZONE_LENGTH (1<<ZONE_SHIFT)
struct page {
	float **p;
	// reservoir region
	void *rr;
	ff_u64_t creation;
	ff_u32_t death;
	ff_mlock_t lock;
	ff_i8_t inuse;
};

float static **map = NULL;
ff_uint_t static xl, yl, zl;
float static **end;
ff_uint_t static page_c = 0;

static struct page *pages = NULL;

ff_u32_t static sched_id;
ff_uint_t static size;

# define lk(__page) \
	(__page)->inuse = 0; \
	ffly_mutex_lock(&(__page)->lock)
# define ul(__page) \
	(__page)->inuse = -1; \
	ffly_mutex_unlock(&(__page)->lock)

void static
page_load(ff_uint_t __page) {
	struct page *pg = pages+__page;
	ff_uint_t size = PAGE_SIZE*sizeof(float);
	float *p;
	p = (*pg->p = (float*)__ffly_mem_alloc(size));
	ffly_reservoir_read(&__ffly_reservoir__, pg->rr, p, 0, size);
	pg->creation = ffly_clock_get();
	pg->death = PAGE_DEATH;
}

float static*
page_get(ff_uint_t __page) {
	float **p = map+__page;
	if (!*p) {
		page_load(__page);
	}
	(pages+__page)->creation = ffly_clock_get();
	return *p;
}

float static*
at(ff_uint_t __x, ff_uint_t __y, ff_uint_t __z, struct page **__page) {
	ff_uint_t off = ((__x)>>ZONE_SHIFT)+(((__y)>>ZONE_SHIFT)*xl)+(((__z)>>ZONE_SHIFT)*(xl*yl));

	ff_uint_t pg = off>>PAGE_SHIFT;
	lk(*__page = pages+pg);
	if (pg>=page_c) {
		return NULL;
	}


	float *p = page_get(pg);
	if (!p) {
		// error
		return NULL;
	}

	return p+(off-(pg*PAGE_SIZE));
}

ff_i8_t static
update(void *__arg) {
	struct page *pg = pages;
	struct page *end = pg+page_c;
	while(pg != end) {
		if (pg->inuse == -1) {
			ffly_mutex_lock(&pg->lock);
			if (ffly_clock_get() >= pg->creation+pg->death && *pg->p != NULL) {
				ffly_reservoir_write(&__ffly_reservoir__, pg->rr, *pg->p, 0, PAGE_SIZE*sizeof(float));
				__ffly_mem_free(*pg->p);	
				*pg->p = NULL;
			}
			ffly_mutex_unlock(&pg->lock);
		}
		pg++;
	}
}

ff_err_t
ffly_gravity_init(ff_uint_t __xl, ff_uint_t __yl, ff_uint_t __zl) {	
	ff_uint_t size;
	size = (xl = (__xl>>ZONE_SHIFT))*(yl = (__yl>>ZONE_SHIFT))*(zl = (__zl>>ZONE_SHIFT));
	page_c = (size>>PAGE_SHIFT)+((size&((~(ff_u64_t)0)>>(64-PAGE_SHIFT)))>0);
	pages = (struct page*)__ffly_mem_alloc(page_c*sizeof(struct page));
	map = (float**)__ffly_mem_alloc(page_c*sizeof(float**));
	end = map+page_c;
	if (!map) {
		reterr;
	}

	void *empty = __ffly_mem_alloc(PAGE_SIZE*sizeof(float));
	ffly_mem_set(empty, 0x0, PAGE_SIZE*sizeof(float));

	struct page *pg;
	float **p = map;
	while(p != end) {
		pg = pages+(p-map);
		pg->p = p;
		pg->rr = ffly_reservoir_alloc(&__ffly_reservoir__, PAGE_SIZE*sizeof(float));
		ffly_reservoir_write(&__ffly_reservoir__, pg->rr, empty, 0, PAGE_SIZE*sizeof(float));
		pg->lock = FFLY_MUTEX_INIT;
		pg->inuse = -1;
		*(p++) = NULL;
	}

	__ffly_mem_free(empty);
	ffly_schedule(update, NULL, 50);
	retok;
}
/*
float static cal_dist(float __x0, float __y0, float __x1, float __y1) {
	float xdis = ffly_abs(__x0-__x1);
	float ydis = ffly_abs(__y0-__y1);
	return (xdis+(ydis*0.5));
}*/

void ffly_gravity_apply(ffly_unip __uni, ffly_phy_bodyp __body, ff_uint_t __delta) {
	ff_uint_t x = (*__body->x)>>ZONE_SHIFT;
	ff_uint_t y = (*__body->y)>>ZONE_SHIFT;
	ff_uint_t z = (*__body->z)>>ZONE_SHIFT;

	if (x+(y*xl)+(z*(xl*yl))>=size) {
		ffly_fprintf(ffly_err, "body out of bounds.\n");
		return;
	}

	float z0;
	float z1;
	float z2;
	float z3;
	float z4;
	float z5;
	float z6;
	float z7;

	z0 = 0.0;
	z1 = 0.0;
	z2 = 0.0;
	z3 = 0.0;
	z4 = 0.0;
	z5 = 0.0;
	z6 = 0.0;
	z7 = 0.0;

	if (_err(ffly_uni_detach_body(__uni, __body))) {
		ffly_printf("failed to detach body.\n");
	}

	struct page *pg;
	if (y>0) {
		z1 = *at(x, y-1, z, &pg);
		ul(pg);
	}
//		z1 = *(map+(x+((y-1)*xl)+(z*(xl*yl))));

	if (x>0) {
		z3 = *at(x-1, y, z, &pg);
		ul(pg);
	}
//		z3 = *(map+((x-1)+(y*xl)+(z*(xl*yl))));

	if (x<xl) {
		z4 = *at(x+1, y, z, &pg);
		ul(pg);
	}
//		z4 = *(map+((x+1)+(y*xl)+(z*(xl*yl))));

	if (y<yl) {
		z6 = *at(x, y+1, z, &pg);
		ul(pg);
	}
//		z6 = *(map+(x+((y+1)*xl)+(z*(xl*yl))));

	if (z1>0.0) {
		float dist = (*__body->y)-((y-1)*ZONE_LENGTH);
		dist+=ZONE_LENGTH/2;
		float force = z1;

		force-=z3/ZONE_LENGTH;
		force-=z4/ZONE_LENGTH;
		force-=z6/(ZONE_LENGTH*2);

		ff_uint_t a = (ff_uint_t)ffly_round((((float)__delta)*0.2)*(((force/dist)*GRAVITY_CONST)/PHY_TIME_PERIOD));
		ffly_printf("-----: %u :\n", a);
		*__body->y-=a;
	}

	if (z3>0.0) {
		float dist = (*__body->x)-((x-1)*ZONE_LENGTH);
		dist+=ZONE_LENGTH/2;
		float force = z3;

		force-=z1/ZONE_LENGTH;
		force-=z4/(ZONE_LENGTH*2);
		force-=z6/ZONE_LENGTH;

		ff_uint_t a = (ff_uint_t)ffly_round((((float)__delta)*0.2)*(((force/dist)*GRAVITY_CONST)/PHY_TIME_PERIOD));
		ffly_printf("-----: %u :\n", a);
		*__body->x-=a;
	}


	if (z4>0.0) {
		float dist = ((x+1)*ZONE_LENGTH)-(*__body->x);
		dist+=ZONE_LENGTH/2;
		float force = z4;
	
		force-=z1/ZONE_LENGTH;
		force-=z3/(ZONE_LENGTH*2);
		force-=z6/ZONE_LENGTH;

		ff_uint_t a = (ff_uint_t)ffly_round((((float)__delta)*0.2)*(((force/dist)*GRAVITY_CONST)/PHY_TIME_PERIOD));
		ffly_printf("-----: %u - %u :\n", a, (ff_uint_t)dist); 
		*__body->x+=a;
	}

	if (z6>0.0) {
		float dist = ((y+1)*ZONE_LENGTH)-(*__body->y);
		dist+=ZONE_LENGTH/2;
		float force = z6;

		force-=z1/(ZONE_LENGTH*2);
		force-=z3/ZONE_LENGTH;
		force-=z4/ZONE_LENGTH;

		ff_uint_t a = (ff_uint_t)ffly_round((((float)__delta)*0.2)*(((force/dist)*GRAVITY_CONST)/PHY_TIME_PERIOD));
		ffly_printf("-----: %u :\n", a);
		*__body->y+=a;
	}

	if (_err(ffly_uni_attach_body(__uni, __body))) {
		ffly_printf("failed to attach body.\n");
	}
}

void ffly_gravity_add(float __val, ff_uint_t __x, ff_uint_t __y, ff_uint_t __z) {
	if (!map) {
		return;
	}
	struct page *pg;
	float *p = at(__x, __y, __z, &pg);
	if (!p) {
		ffly_fprintf(ffly_err, "gravity add error.\n");
		return;
	}
	*p+=__val;
	ul(pg);
}

void ffly_gravity_sub(float __val, ff_uint_t __x, ff_uint_t __y, ff_uint_t __z) {
	if (!map) {
		return;
	}
	struct page *pg;
	float *p = at(__x, __y, __z, &pg);
	if (!p) {
		ffly_fprintf(ffly_err, "gravity sub error.\n");
		return;
	}
	*p-=__val;
	ul(pg);
}

/*
	get gravity at point
*/
float ffly_gravity_at(ff_uint_t __x, ff_uint_t __y, ff_uint_t __z) {
	if (!map) {
		return 0.0;
	}
	struct page *pg;
	float *p = at(__x, __y, __z, &pg);
	if (!p) {
		ffly_fprintf(ffly_err, "gravity at error.\n");
		return 0.0;
	}

	float ret;
	ret = *p;
	ul(pg);
	return ret;
}

void ffly_gravity_cleanup() {
	ffly_sched_rm(sched_id);
	float **p = map;
	while(p != end) {
		if (*p != NULL) {
			__ffly_mem_free(*p);
		}
		p++;
	}

	__ffly_mem_free(map);
	__ffly_mem_free(pages);
}
