# include "str_cmp.h"
# include "mem_cmp.h"
# include "str_len.h"
# include "../types/size_t.h"
ffly_byte_t ffly_str_cmp(char const *__s1, char const *__s2) {
	ffly_size_t s1_len = ffly_str_len(__s1);
	ffly_size_t s2_len = ffly_str_len(__s2);
	if (s1_len != s2_len) return 0;

	return ffly_mem_cmp((mdl_u8_t*)__s1, (mdl_u8_t*)__s2, s1_len);
}
