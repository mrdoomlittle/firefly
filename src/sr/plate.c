# include "plate.h"
# include "def.h"
# define N 1024
static struct sr_plate plates[N];
static struct sr_plate *next = plates;
static struct sr_plate *free = 	NULL;

struct sr_plate static*
plate_alloc(void) {
	struct sr_plate *p;
	if (free != NULL) {
		p = free;
		free = free->link;
	} else
		p = next++;
	return p;
}

void static
plate_free(struct sr_plate *__plate) {
	if (__plate == next-1) {
		next--;
	} else {
		__plate->link = free;
		free = __plate;
	}
}

ff_u16_t sr_plate_alloc(void) {
	return plate_alloc()-plates;
}

void sr_plate_free(ff_u16_t __plate) {
	plate_free(plates+__plate);
}

void sr_plate_put(ff_u16_t __plate, void *__p) {
	(plates+__plate)->p = __p;
}

void* sr_plate_get(ff_u16_t __plate) {
	return (plates+__plate)->p;
}
