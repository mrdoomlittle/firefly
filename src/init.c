# include "init.h"
# include "system/io.h"
# include "system/config.h"
# include "dep/str_cpy.h"
# include "exec.h"
# include "linux/unistd.h"
# include "system/error.h"
# include "system/log.h"
void ffly_init_run() {
	if (!__ffly_sysconf__.root_dir ||
		!__ffly_sysconf__.inidir ||
		!__ffly_sysconf__.inil) return;

	if (!*__ffly_sysconf__.inil) return;
	char buf[1024];
	char *bufp = buf;
	bufp+=ffly_str_cpy(bufp, __ffly_sysconf__.root_dir);
	*(bufp++) = '/';
	bufp+=ffly_str_cpy(bufp, __ffly_sysconf__.inidir);
	*(bufp++) = '/';
	char const **ini = __ffly_sysconf__.inil;
	while(*ini != NULL) {
		ffly_str_cpy(bufp, *ini);
		log("init path: %s\n", buf);
		if (access(buf, F_OK) == -1) {
			log("file access has been denied, or file doesent exist. skipping...\n", buf);
			goto _sk;
		}
		ffexecf(buf);
	_sk:
		ini++;
	}
}

# include "system/sched.h"
# include "storage/reservoir.h"
// remove CORRODE from flags

struct init_arg *__init_arg__;
void static
sched_init(void) {
	log("sched init, flags: %u\n", __init_arg__->flags);
	ffly_scheduler_init(__init_arg__->flags|SCHED_CORRODE);
}

void static
resv_init(void) {
	log("resv init, flags: %u\n", __init_arg__->flags);
	ffly_reservoir_init(&__ffly_reservoir__, __init_arg__->flags|RESV_CORRODE, "temp.resv");
}

# define MAX 2
static void(*init[])(void) = {
	sched_init,
	resv_init
};

ff_err_t
ffly_init(ff_u8_t __what) {
	if (__what>= MAX) {
		// error
		return FF_ERR_NEIP;
	}

	if (!__init_arg__) {
		log("init arg is null - might cause an error.\n");
	}
	init[__what]();
	retok;
}
