# include "str_len.h"
mdl_uint_t ffly_str_len(char *__s) {
	if (!__s) return 0;
	char *itr = __s;
	while(*(itr++) != '\0');
	return itr-__s;
}
