# include "driver.h"
# include "../drivers/sr.h"
static void(*load_driver[])(struct bron_driver*) = {
	ffly_sr
};

void ffly_bron_driver(ff_u8_t __id, struct bron_driver *__driver) {
	load_driver[__id](__driver);
}
