# include "as.h"
# include <stdio.h>
# include "../data/str_cmp.h"
symbolp parse(char *__s) {
	char *p = __s;

	mdl_u8_t sec = 0;
	mdl_uint_t len;
	while(*p == ' ') p++;
	char buf[128], *bufp;
	bufp = buf;
	char *s = NULL;

	symbolp sy = _alloca(sizeof(struct symbol)), cur;
	if (*p == '.') {
		sec = 1;
		p++;
		while(*p >= 'a' && *p <= 'z') {
			*(bufp++) = *(p++); 
		}
		*bufp = '\0';
	} else if (*p == '%') {
		p++;
		sy->flags = SY_MAC;
	}

	if (sec) {
		if (!ffly_str_cmp(buf, "section")) {
			p++;
			s = read_str(p, &len);
			p+=len;
			sy->len = len;
		}

		printf("[%s], [%s]\n", buf, s);
		return NULL;
	}

	sy->p = read_str(p, &len);
	p+= len;
	sy->len = len;

	if (*p == ':') {
		// label
		goto _ret;
	}

	sy->next = eval(p);
/*
	cur = sy;
	while(cur != NULL) {
		printf("%s\n", cur->p);
		cur = cur->next;
	}
*/		
	_ret:
	return sy;
}
