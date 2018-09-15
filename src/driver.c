# include "driver.h"
# include "drivers/sr.h"
static void(*load_driver[])(struct ff_driver*) = {
	ffly_sr
};

void ffly_driver(ff_u8_t __id, struct ff_driver *__driver) {
	load_driver[__id](__driver);
}
