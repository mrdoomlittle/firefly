# include "pp.h"
# include "../malloc.h"

bucketp static bk = NULL;
ff_u8_t static buf[128];

ff_u8_t static
is_space() {
	return (*p == ' ' || *p == '\t');
}

void sk_space() {
	while(is_space()) {
		if (at_eof()) return;
		p++;
	}
}

bucketp static feed = NULL;

void ulex(bucketp __p) {
	feed = __p;
}

bucketp lex() {
	if (feed != NULL) {
		bucketp pre = feed;
		feed = NULL;
		return pre;
	}

	if (at_eof())
		return NULL;

	bucketp ret;
	if (!(ret = malloc(sizeof(struct bucket)))) {
		return NULL;
	}

	if (!bk)
		goto _j;
	if (bk->sort == _percent) {
		sk_space();
		ret->sort = _ident;
		ret->p = buf;
		ff_u8_t *bufp = buf;
		while((*p >= 'a' && *p <= 'z') || *p == '_')
			*(bufp++) = *(p++);
		*bufp = '\0';
	} else {
		_j:
		if (*p == '%') {
			ret->sort = _percent;
		} else {
			ret->val = *p;
			ret->sort = _chr;
		}
		p++;
	}

	if (bk != NULL)
		free(bk);
	bk = ret;

	return ret;
}
