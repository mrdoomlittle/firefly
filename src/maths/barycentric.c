# include "barycentric.h"
# include "dot.h"
void ffly_barycentric(mdl_int_t __x, mdl_int_t __y, mdl_int_t __z, ffly_vertexp __a, ffly_vertexp __b, ffly_vertexp __c, float *__alpha, float *__beta, float *__gamma) {
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

    float d00 = ffly_dot(v0.x, v0.x, v0.y, v0.y, v0.z, v0.z);
    float d01 = ffly_dot(v0.x, v1.x, v0.y, v1.y, v0.z, v1.z);
    float d11 = ffly_dot(v1.x, v1.x, v1.y, v1.y, v1.z, v1.z);
    float d02 = ffly_dot(v0.x, v2.x, v0.y, v2.y, v0.z, v2.z);
    float d12 = ffly_dot(v1.x, v2.x, v1.y, v2.y, v1.z, v2.z);    
    float den = 1.0/(d00*d11-d01*d01);
    *__alpha = (d11*d02-d01*d12)*den;
    *__beta = (d00*d12-d01*d02)*den;
    *__gamma = (1.0-(*__alpha))-(*__beta);
}

