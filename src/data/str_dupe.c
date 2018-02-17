# include "str_dupe.h"
# include "mem_dupe.h"
# include "str_len.h"
# include "../ffly_def.h"
char* ffly_str_dupe(char const *__s) {
	void *p = NULL;
	ffly_mem_dupe(&p, __s, ffly_str_len(__s)+1);
	return (char*)p;
}
