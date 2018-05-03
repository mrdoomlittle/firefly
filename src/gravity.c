# include "gravity.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "physics/clock.h"
# include "maths/round.h"
# include "system/io.h"

ff_err_t ffly_uni_attach_body(ffly_unip, ffly_phy_bodyp);
ff_err_t ffly_uni_detach_body(ffly_unip, ffly_phy_bodyp);
/*
	to reduce memory
*/
# define ZONE_SHIFT 5
# define ZONE_LENGTH (1<<ZONE_SHIFT)
# define at(__x, __y, __z) \
	(map+(((__x)>>ZONE_SHIFT)+(((__y)>>ZONE_SHIFT)*xl)+(((__z)>>ZONE_SHIFT)*(xl*yl))))

float static *map = NULL;
ff_uint_t static xl, yl, zl;
void ffly_gravity_init(ff_uint_t __xl, ff_uint_t __yl, ff_uint_t __zl) {	
	ff_uint_t size;
	map = (float*)__ffly_mem_alloc((size = ((xl = (__xl>>ZONE_SHIFT))*(yl = (__yl>>ZONE_SHIFT))*(zl = (__zl>>ZONE_SHIFT))))*sizeof(float));
	float *p = map;
	float *end = map+size;
	while(p != end)
		*(p++) = 0.0;
}

# define CONST  0.01
void ffly_gravity_apply(ffly_unip __uni, ffly_phy_bodyp __body, ff_uint_t __delta) {
	ff_uint_t x = (*__body->x)>>ZONE_SHIFT;
	ff_uint_t y = (*__body->y)>>ZONE_SHIFT;
	ff_uint_t z = (*__body->z)>>ZONE_SHIFT;

	float *z0;
	float *z1;
	float *z2;
	float *z3;
	float *z4;
	float *z5;
	float *z6;
	float *z7;

	/*
		only experimenting with this
	*/
	ffly_uni_detach_body(__uni, __body);
	if (y>0) {
		z1 = map+(x+((y-1)*xl)+(z*(xl*yl)));
		float dist = (*__body->y)-((y-1)*ZONE_LENGTH);
		ff_uint_t a = (ff_uint_t)ffly_round((((float)(__delta>>1))*0.2)*((((*z1)/dist)*CONST)/TIME_PERIOD)); 
		ffly_printf("------ %u\n", a);
		*__body->y-=a;
	}

	if (x>0) {
	}

	if (x<xl) {
	}

	if (y<yl) {
	}

	ffly_uni_attach_body(__uni, __body);
}

void ffly_gravity_add(float __val, ff_uint_t __x, ff_uint_t __y, ff_uint_t __z) {
	float *p = at(__x, __y, __z);
	*p+=__val;
}

void ffly_gravity_sub(float __val, ff_uint_t __x, ff_uint_t __y, ff_uint_t __z) {
	float *p = at(__x, __y, __z);
	*p-=__val;
}

float ffly_gravity_at(ff_uint_t __x, ff_uint_t __y, ff_uint_t __z) {
	return *at(__x, __y, __z);
}

void ffly_gravity_cleanup() {
	__ffly_mem_free(map);
}
