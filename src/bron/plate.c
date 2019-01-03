# include "plate.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"

struct plate {
	ff_u32_t n;
	struct plate *next;
};

static struct plate *free = NULL;

static ff_u32_t next_plate = 0;
ff_u32_t bron_plate_alloc(void) {
	if (free != NULL) {
		struct plate *p;
		p = free;
		free = free->next;
		ff_u32_t n;
		n = p->n;
		__ffly_mem_free(p);
		return n;
	}

	return next_plate++;
}

void bron_plate_free(ff_u32_t __pn) {
	if (next_plate == __pn-1) {
		next_plate--;
		return;
	}
	struct plate *p;
	p = (struct plate*)__ffly_mem_alloc(sizeof(struct plate));


	p->next = free;
	free = p;
	p->n = __pn;
}
