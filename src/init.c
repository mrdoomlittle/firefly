# include "system/io.h"
# include "system/config.h"
# include "dep/str_cpy.h"
# include "exec.h"
void ffly_init() {
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
		ffexecf(buf);
		ffly_printf("init path: %s\n", buf);
		ini++;
	}
}
