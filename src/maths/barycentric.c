# include "barycentric.h"
# include "dot.h"
void ffly_barycentric(ff_int_t __x, ff_int_t __y, ff_int_t __z, ffly_vertexp __a, ffly_vertexp __b, ffly_vertexp __c, float *__alpha, float *__beta, float *__gama) {
	ffly_vertex v0 = {
		.x=__b->x-__a->x,
		.y=__b->y-__a->y,
		.z=__b->z-__a->z
	};

	ffly_vertex v1 = {
		.x=__c->x-__a->x,
		.y=__c->y-__a->y,
		.z=__c->z-__a->z
	};

	ffly_vertex v2 = {
		.x=__x-__a->x,
		.y=__y-__a->y,
		.z=__z-__a->z
	};

	float d00, d01, d11, d02, d12, den;
	d00 = ffly_dot(v0.x, v0.x, v0.y, v0.y, v0.z, v0.z);
	d01 = ffly_dot(v0.x, v1.x, v0.y, v1.y, v0.z, v1.z);
	d11 = ffly_dot(v1.x, v1.x, v1.y, v1.y, v1.z, v1.z);
	d02 = ffly_dot(v0.x, v2.x, v0.y, v2.y, v0.z, v2.z);
	d12 = ffly_dot(v1.x, v2.x, v1.y, v2.y, v1.z, v2.z);	
	den = 1.0/(d00*d11-d01*d01);


	float a, b, g;
	a = (d11*d02-d01*d12)*den;
	b = (d00*d12-d01*d02)*den;
	*__gama = (1.0-a)-b;
	*__alpha = a;
	*__beta = b;
}

