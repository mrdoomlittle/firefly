# include "plate.h"
# include "def.h"
# define N 1024
static struct nt_plate plates[N];
static struct nt_plate *next = plates;
static struct nt_plate *free = 	NULL;

struct nt_plate static*
plate_alloc(void) {
	struct nt_plate *p;
	if (free != NULL) {
		p = free;
		free = free->link;
	} else
		p = next++;
	return p;
}

void static
plate_free(struct nt_plate *__plate) {
	if (__plate == next-1) {
		next--;
	} else {
		__plate->link = free;
		free = __plate;
	}
}

ff_u16_t nt_plate_alloc(void) {
	return plate_alloc()-plates;
}

void nt_plate_free(ff_u16_t __plate) {
	plate_free(plates+__plate);
}

void nt_plate_put(ff_u16_t __plate, void *__p) {
	(plates+__plate)->p = __p;
}

void* nt_plate_get(ff_u16_t __plate) {
	return (plates+__plate)->p;
}
