# include "as.h"
# include "../stdio.h"
# include "../data/str_cmp.h"
symbolp parse(char *__s) {
	char *p = __s;

	mdl_u8_t dir = 0;
	mdl_uint_t len;
	while(*p == ' ') p++;
	char buf[128], *bufp;
	bufp = buf;
	char *s = NULL;

	symbolp sy = _alloca(sizeof(struct symbol)), cur;
	if (*p == '.') {
		dir = 1;
		p++;
		while(*p >= 'a' && *p <= 'z') {
			*(bufp++) = *(p++); 
		}
		*bufp = '\0';

		sy->sort = SY_DIR;
		sy->p = memdup(buf, (sy->len = (bufp-buf)+1));
		sy->next = _alloca(sizeof(struct symbol));
		sy = sy->next;
	} else if (*p == '%') {
		p++;
		sy->sort = SY_MAC;
	}

	if (dir) {
		p++; //space
		s = read_str(p, &len);
		p+=len;
		sy->sort = SY_DIR;
		sy->p = s;
		sy->len = len;
		return sy;
	}

	sy->p = read_str(p, &len);
	p+= len;
	sy->len = len;

	if (*p == ':') {
		// label
		sy->sort = SY_LABEL;
		goto _ret;
	} else
		sy->sort = SY_STR;	

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
