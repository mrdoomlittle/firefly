# include "fs.h"
void posix_fsop(struct ffly_fsop*);

/*
	i know i could just load the structure and not call a function to load it for me but
	then on the other end if we wanted to setup some stuff we dont be able too.
*/
static void(*load[])(struct ffly_fsop*) = {
	[FF_POSIX_FS] = posix_fsop
};

static struct ffly_fsop _sysfs;
struct ffly_fsop *sysfs;
void ffly_sysfs(ff_u8_t __what) {
	load[__what](&_sysfs);
	sysfs = &_sysfs;
}
