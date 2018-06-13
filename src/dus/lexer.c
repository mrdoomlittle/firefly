# include "dus.h"
# include "../malloc.h"
# include "../stdio.h"
# include "../string.h"
# define nextc (*p)

ff_u8_t is_space(char __c) {
	return __c == ' ' || __c == '\n';
}

void static
sk_white_space(void) {
	while(is_space(nextc)) incrp;
}

char static*
read_ident(ff_uint_t *__l) {
	char buf[128];
	char *bufp = buf;
	char c = nextc;
	while(c>='a' && c<='z') {
		*(bufp++) = c;
		incrp;
		c = nextc;
	}
	
	*bufp = '\0';
	ff_uint_t l;
	char *p = (char*)malloc((l = (bufp-buf))+1);
	memcpy(p, buf, l+1);
	*__l = l;
	return p;
}

char static*
read_str(ff_uint_t *__l) {
	char buf[128];
	char *bufp = buf;
	char c = nextc;
	while(c != '"') {
		*(bufp++) = c;
		incrp;
		c = nextc;
	}

	*bufp = '\0';
	ff_uint_t l;
	char *p = (char*)malloc((l = (bufp-buf))+1);
	memcpy(p, buf, l+1);
	*__l = l;
	return p;
}

tokenp peektok(void) {
	tokenp ret;
	ff_dus_ulex(ret = ff_dus_nexttok());
	return ret;
}

# define BUFSIZE 20
tokenp static buf[BUFSIZE];
tokenp static* end = buf;

void ff_dus_ulex(tokenp __p) {
	*(end++) = __p;
}

# define BACK 10
tokenp static head = NULL;
tokenp static next = NULL;
ff_uint_t static len = 0;

tokenp ff_dus_lex(void) {
	if (end>buf)
		return *(--end);
	if (len > BACK && head != NULL) {
		len--;
		tokenp bk = head;
		head = head->next;
		free(bk);
	}

	sk_white_space();

	if (at_eof()) {
		return NULL;
	}

	tokenp tok;
	tok = (tokenp)malloc(sizeof(struct token));
	tok->p = NULL;
	tok->next = NULL;
	if (!head)
		head = tok;
	if (next != NULL)
		next->next = tok;
	next = tok;

	char c = nextc;
	if (c>='a' && c<='z') {
		tok->sort = _tok_ident;
		tok->p = read_ident(&tok->l);
	} else {
		switch(c) {
			case '"':
				incrp;
				tok->sort = _tok_str;
				tok->p = read_str(&tok->l);
				incrp;
			break;
			case '=':
				tok->sort = _tok_keywd;
				tok->val = _eq;
				incrp;				
			break;
			default:
				printf("unknown %c\n", c);
		}
	}
	len++;
	return tok;
}
