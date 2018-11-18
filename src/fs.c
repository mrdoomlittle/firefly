# include "fs.h"
# include "fs/m/m.h"
struct ff_fso *__ff_fso__;

static void(*load[])(void) = {
	ffly_mfs
};

static void(*de_init[])(void) = {
	ffly_mfs_de_init
};

void(*ffly_fs(ff_u8_t __what))(void) {
	load[__what]();
	return de_init[__what];
}
