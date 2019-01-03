# include "tile.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "def.h"
# include "../dep/mem_set.h"
# include "raise.h"
# include "plate.h"
void nt_ptile_new(void) {
	ff_u32_t d;
	d = *(ff_u32_t*)nt_raise_p;

	void *g, *t;
	g = *(void**)(nt_raise_p+4);
	t = *(void**)(nt_raise_p+12);

	struct nt_ptile *pt;

	pt = (struct nt_ptile*)__ffly_mem_alloc(sizeof(struct nt_ptile));

	nt_plate_put(d, pt);

	pt->get = (void(*)(ff_u8_t, long long, void*))g;
	pt->tile = t;
}

void nt_ptile_destroy(void) {
	ff_u32_t plate;
	plate = *(ff_u32_t*)nt_raise_p;

	struct nt_ptile *pt;
	pt = (struct nt_ptile*)nt_plate_get(plate);
	__ffly_mem_free(pt);
}

struct nt_tile* nt_tile_new(ff_u8_t __sz) {
	struct nt_tile *t;
	t = (struct nt_tile*)__ffly_mem_alloc(sizeof(struct nt_tile));
	t->sz = __sz;
	t->bits = 0x00;
	t->map = NULL;
	return t;
}

void nt_tile_map(struct nt_tile *__t) {
	ff_uint_t sz;
	__t->map = __ffly_mem_alloc(sz = ((1<<(__t->sz<<1/*x2*/))*4));
	ffly_mem_set(__t->map, 0, sz);
}

void nt_tile_unmap(struct nt_tile *__t) {
	__ffly_mem_free(__t->map);
	__t->map = NULL;
}

void nt_tile_destroy(struct nt_tile *__t) {
	if (__t->map != NULL) {
		nt_tile_unmap(__t);
	}
	__ffly_mem_free(__t);
}
