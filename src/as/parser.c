# include "as.h"
# include "../stdio.h"
# include "../dep/str_cmp.h"
symbolp ff_as_parse(char *__s) {
	char *p = __s;

	ff_u8_t dir = 0;
	ff_uint_t len;
	while(*p == ' ' && *p == '\t') p++;
	char buf[128], *bufp;
	bufp = buf;
	char *s = NULL;

	symbolp sy = ff_as_al(sizeof(struct symbol)), cur;
	cur = sy;
	if (*p == '.') {
		dir = 1;
		p++;
		while(*p >= 'a' && *p <= 'z')
			*(bufp++) = *(p++);
		*bufp = '\0';

		cur->sort = SY_DIR;
		cur->p = ff_as_memdup(buf, (cur->len = (bufp-buf))+1);
		cur->next = ff_as_al(sizeof(struct symbol));
		cur = cur->next;
	} else if (*p == '%') {
		p++;
		cur->sort = SY_MAC;
	}

	if (dir) {
		p++; //space
		bufp = buf;
		while((*p >= 'a' && *p <= 'z') || *p == '_' || (*p >= '0' && *p <= '9'))
			*(bufp++) = *(p++);
		*bufp = '\0';
		s = ff_as_memdup(buf, (len = (bufp-buf))+1);
		cur->sort = SY_DIR;
		cur->p = s;
		cur->len = len;
		return sy;
	}

	bufp = buf;
	while((*p >= 'a' && *p <= 'z') || *p == '_' || (*p >= '0' && *p <= '9'))
		*(bufp++) = *(p++);
	if (bufp>buf+1) {
		if (!ff_as_suffix(*(bufp-1)))
			bufp--;
	}
	
	*bufp = '\0';

	cur->p = ff_as_memdup(buf, (len = (bufp-buf))+1);
	cur->len = len;

	if (*p == ':') {
		// label
		cur->sort = SY_LABEL;
		goto _ret;
	} else
		cur->sort = SY_STR;	

	// skip spaces
	while(*p == ' ') p++;

	// check if end of line
	if (*p == '\0' || *p == '\n' || *p == ';') {
		cur->next = NULL;
		goto _sk;
	}
	cur->next = ff_as_eval(p);
_sk:
	cur = sy;
	while(cur != NULL) {
		printf(":: %u\n", cur->sort);
		cur = cur->next;
	}

_ret:
	return sy;
}
