# include "as.h"
# include "../stdio.h"
# include "../dep/str_cmp.h"
symbolp parse(char *__s) {
	char *p = __s;

	ff_u8_t dir = 0;
	ff_uint_t len;
	while(*p == ' ' && *p == '\t') p++;
	char buf[128], *bufp;
	bufp = buf;
	char *s = NULL;

	symbolp sy = _alloca(sizeof(struct symbol)), cur;
	cur = sy;
	if (*p == '.') {
		dir = 1;
		p++;
		while(*p >= 'a' && *p <= 'z')
			*(bufp++) = *(p++);
		*bufp = '\0';

		cur->sort = SY_DIR;
		cur->p = _memdup(buf, (cur->len = (bufp-buf))+1);
		cur->next = _alloca(sizeof(struct symbol));
		cur = cur->next;
	} else if (*p == '%') {
		p++;
		cur->sort = SY_MAC;
	}

	if (dir) {
		p++; //space
		bufp = buf;
		while(*p >= 'a' && *p <= 'z')
			*(bufp++) = *(p++);
		*bufp = '\0';
		s = _memdup(buf, (len = (bufp-buf))+1);
		cur->sort = SY_DIR;
		cur->p = s;
		cur->len = len;
		return sy;
	}

	bufp = buf;
	while((*p >= 'a' && *p <= 'z') | *p == '_' | (*p >= '0' && *p <= '9'))
		*(bufp++) = *(p++);
	*bufp = '\0';

	cur->p = _memdup(buf, (len = (bufp-buf))+1);
	cur->len = len;

	if (*p == ':') {
		// label
		cur->sort = SY_LABEL;
		goto _ret;
	} else
		cur->sort = SY_STR;	

	cur->next = eval(p);
/*
	cur = sy;
	while(cur != NULL) {
		printf("%u\n", *(ff_u8_t*)cur->p);
		cur = cur->next;
	}
*/		
_ret:
	return sy;
}
