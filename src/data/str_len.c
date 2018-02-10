# include "str_len.h"
mdl_uint_t ffly_str_len(char const *__s) {
	if (!__s) return 0;
	char const *p = __s;
	while(*(p++) != '\0');
	return (p-__s)-1;
}
