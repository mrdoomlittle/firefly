# include "inet.h"
# include "system/string.h"
mdl_u32_t inet_addr(char const *__p)  {
	char const *p = __p;
	mdl_u32_t ret = 0;
	char buf[4];

	char *bufp = buf;
	_again:
	if (*p == '.' || *p == '\0') {
		*bufp = '\0';
		ret = ((mdl_u32_t)ffly_stno(buf)<<24|ret>>8);
		if (*p != '\0') {
			bufp = buf;
			p++;
			goto _again;
		}
	} else {
		*(bufp++) = *(p++);
		goto _again;
	}
	return ret;
}
