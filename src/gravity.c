# include "gravity.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "physics/clock.h"
# include "maths/round.h"
# include "system/io.h"

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
# define ZONE_SHIFT 4

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

	/*
		only experimenting with this
	*/
	ffly_uni_detach_body(__uni, __body);
	if (y>0)
		z1 = *(map+(x+((y-1)*xl)+(z*(xl*yl))));

	if (x>0)
		z3 = *(map+((x-1)+(y*xl)+(z*(xl*yl))));

	if (x<xl)
		z4 = *(map+((x+1)+(y*xl)+(z*(xl*yl))));

	if (y<yl)
		z6 = *(map+(x+((y+1)*xl)+(z*(xl*yl))));

	if (z1>0.0) {
		float dist = (*__body->y)-((y-1)*ZONE_LENGTH);
		dist+=ZONE_LENGTH/2;
		float force = z1;

		force-=z3/ZONE_LENGTH;
		force-=z4/ZONE_LENGTH;
		force-=z6/(ZONE_LENGTH*2);

		ff_uint_t a = (ff_uint_t)ffly_round((((float)__delta)*0.2)*(((force/dist)*GRAVITY_CONST)/TIME_PERIOD));
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

		ff_uint_t a = (ff_uint_t)ffly_round((((float)__delta)*0.2)*(((force/dist)*GRAVITY_CONST)/TIME_PERIOD));
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

		ff_uint_t a = (ff_uint_t)ffly_round((((float)__delta)*0.2)*(((force/dist)*GRAVITY_CONST)/TIME_PERIOD));
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

		ff_uint_t a = (ff_uint_t)ffly_round((((float)__delta)*0.2)*(((force/dist)*GRAVITY_CONST)/TIME_PERIOD));
		ffly_printf("-----: %u :\n", a);
		*__body->y+=a;
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
