# include "plate.h"
# include "def.h"
# include "../system/io.h"
#define N 4024
static void *plates[N];

void nt_plate_put(ff_u32_t __plate, void *__p) {
	if (__plate>=N) {
		ffly_printf("error. got : %u\n", __plate);
		return;
	}
	plates[__plate] = __p;
}

void* nt_plate_get(ff_u32_t __plate) {
	if (__plate>=N) {
		ffly_printf("error. got : %u\n", __plate);
		return NULL;
	}
	return plates[__plate];
}
