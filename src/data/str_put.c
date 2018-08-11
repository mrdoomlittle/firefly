# include "str_put.h"
# include "../dep/str_len.h"
# include "../dep/mem_cpy.h"
ff_uint_t ffly_str_put(char const *__src, char *__dst) {
	ff_uint_t len;

	len = ffly_str_len(__src);
	ffly_mem_cpy(__dst, __src, len);
	return len;
}
