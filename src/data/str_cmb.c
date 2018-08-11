# include "str_cmb.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../dep/str_len.h"
# include "../dep/mem_cpy.h"
char const*
ffly_str_cmb(char const *__s0, char const *__s1) {
	ff_uint_t s0_len, s1_len, l;

	l = (s0_len = ffly_str_len(__s0))
		+(s1_len = ffly_str_len(__s1));

	char *p;
	p = (char*)__ffly_mem_alloc(l+1);
	ffly_mem_cpy(p, __s0, s0_len);
	ffly_mem_cpy(p+s0_len, __s1, s1_len);
	*(p+l) = '\0';
	return (char const*)p;
}
