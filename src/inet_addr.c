# include "inet.h"
# include "system/string.h"
mdl_u32_t inet_addr(char const *__p)  {
	char const *p = __p;
	mdl_u32_t ret;
	char buf[4];

	char *bufp = buf;
	while(*p != '\0') {
		*(bufp++) = *(p++);	
		if (*p == '.' || *p == '\0') {
			ret = (((mdl_u32_t)ffly_stno(buf))<<24|(ret&0xffffff))>>8;
			*bufp = '\0';
			bufp = buf;
		}
	}
	return ret;
}
