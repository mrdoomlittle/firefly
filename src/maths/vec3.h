# ifndef __ffly__vec3__h
# define __ffly__vec3__h

typedef struct ffly_vec3 {
	float x, y, z;
} *ffly_vec3p;

void ffly_vec3_clr(ffly_vec3p);
float ffly_vec3_dist(const ffly_vec3p, const ffly_vec3p);
void ffly_vec3_rotate(ffly_vec3p, float);
# endif /*__ffly__vec3__h*/
