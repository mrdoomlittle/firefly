# include "gf.h"
# include "bron/tri.h"

void static
_tri2(struct ffly_tri2 *__tri, ff_u16_t __tex, ff_u32_t __x, ff_u32_t __y) {
	struct bron_tri2 tri;
	
	tri = *(struct bron_tri2*)&__tri->coords;
	bron_tri2(&tri, __tex, __x, __y);
}

void static
_tri3(struct ffly_tri3 *__tri, ff_u16_t __tex, ff_u32_t __x, ff_u32_t __y, ff_u32_t __z) {
	struct bron_tri3 tri;

	tri = *(struct bron_tri3*)&__tri->coords;
	bron_tri3(&tri, __tex, __x, __y, 0);
}

static struct ffly_gf _gf = {
	_tri2,
	_tri3
};

struct ffly_gf *gf = &_gf;
void ffly_gfload(void) {
//	gf = &_gf;
}
