# include "vec2.h"
# include "abs.h"
void ffly_vec2_clr(ffly_vec2p __vec) {
	__vec->x = 0.0;
	__vec->y = 0.0;
}

/*
	i think this works, for now we will use this as its faster but less accurate.
	will be replaced until needed.
*/
float ffly_vec2_dist(const ffly_vec2p __a, const ffly_vec2p __b) {
	float xdis = ffly_abs(__a->x-__b->x);
	float ydis = ffly_abs(__a->y-__b->y);
	return (xdis+(ydis*0.5));
}

void ffly_vec2_rotate(ffly_vec2p __vec, float __angle) {
	
}
