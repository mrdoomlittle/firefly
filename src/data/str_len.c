# include "str_len.h"
mdl_uint_t ffly_str_len(char *__s) {
	char *itr = __s;
	while(*(++itr) != '\0');
	return itr-__s;
}
