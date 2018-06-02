# include "init.h"
# include "system/io.h"
# include "system/config.h"
# include "dep/str_cpy.h"
# include "exec.h"
# include "linux/unistd.h"
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
		ffly_fprintf(ffly_log, "init path: %s\n", buf);
		if (access(buf, F_OK) == -1) {
			ffly_fprintf(ffly_log, "file access has been denied, or file doesent exist. skipping...\n", buf);
			goto _sk;
		}
		ffexecf(buf);
	_sk:
		ini++;
	}
}

# include "system/sched.h"
struct init_arg *__init_arg__;
void static
sched_init(void) {
	ffly_printf("sched init, flags: %u\n", __init_arg__->flags);
	ffly_scheduler_init(__init_arg__->flags);
}

static void(*init[])(void) = {
	sched_init
};

void ffly_init(ff_u8_t __what) {
	init[__what]();
}
