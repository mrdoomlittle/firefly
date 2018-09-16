# include "tile.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "def.h"
# include "../dep/mem_set.h"
# include "raise.h"
# include "plate.h"
void sr_ptile_new(void) {
	ff_u16_t *d;
	d = (ff_u16_t*)stack_at(*(ff_u16_t*)sr_raise_p);

	void *g, *t;
	g = *(void**)(sr_raise_p+2);
	t = *(void**)(sr_raise_p+10);

	struct sr_ptile *pt;

	pt = (struct sr_ptile*)__ffly_mem_alloc(sizeof(struct sr_ptile));

	*d = sr_plate_alloc();
	sr_plate_put(*d, pt);

	pt->get = (void(*)(ff_u8_t, long long, void*))g;
	pt->tile = t;
}

void sr_ptile_destroy(void) {
	ff_u16_t plate;
	plate = *(ff_u16_t*)sr_raise_p;

	struct sr_ptile *pt;
	pt = (struct sr_ptile*)sr_plate_get(plate);
	__ffly_mem_free(pt);
}

struct sr_tile* sr_tile_new(ff_u8_t __sz) {
	struct sr_tile *t;
	t = (struct sr_tile*)__ffly_mem_alloc(sizeof(struct sr_tile));
	t->sz = __sz;
	t->bits = 0x00;
	t->map = NULL;
	return t;
}

void sr_tile_map(struct sr_tile *__t) {
	ff_uint_t sz;
	__t->map = __ffly_mem_alloc(sz = ((1<<(__t->sz<<1))*4));
	ffly_mem_set(__t->map, 0, sz);
}

void sr_tile_unmap(struct sr_tile *__t) {
	__ffly_mem_free(__t->map);
	__t->map = NULL;
}

void sr_tile_destroy(struct sr_tile *__t) {
	if (__t->map != NULL) {
		sr_tile_unmap(__t);
	}
	__ffly_mem_free(__t);
}
