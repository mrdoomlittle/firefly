# ifndef __ffly__vec2__h
# define __ffly__vec2__h

typedef struct ffly_vec2 {
	float x, y;
} *ffly_vec2p;

void ffly_vec2_clr(ffly_vec2p);
float ffly_vec2_dist(const ffly_vec2p, const ffly_vec2p);
void ffly_vec2_rotate(ffly_vec2p, float);
# endif /*__ffly__vec2__h*/
