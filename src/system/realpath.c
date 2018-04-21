# include "realpath.h"
# include <linux/limits.h>
# include "../linux/unistd.h"
# include "../memory/mem_alloc.h"
# include "../dep/str_cpy.h"
# include "../stdio.h"
char *ffly_realpath(char const *__path) {
	char *buf, *end;
	buf = (char*)__ffly_mem_alloc(PATH_MAX);
	getcwd(buf, PATH_MAX);
	end = buf;
	while(*end != '\0') end++;

	char const *p = __path;
	if (*p != '.') {

	} else {
	_again:
		while(*p != '/') {
			if (*p == '.') {
				if (*(p++) == '.') {
					while(*end != '/') *(end--) = '\0';	
					*end = '\0';
				}
				p++;
			}
		}
		if (*(p+1) == '.') {
			p++;
			goto _again;
		}
	}

	ffly_str_cpy(end, p);
	return buf;
}
