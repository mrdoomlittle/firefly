# include "dus.h"
# include "../malloc.h"
# include "../stdio.h"
# include "../string.h"
#define nextc (*dus_p)

ff_u8_t is_space(char __c) {
	return __c == ' ' || __c == '\n' || __c == '\t';
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
	while((c>='a' && c<='z') || c == '_') {
		if ((bufp-buf)>=128) {
			// buffer overflow
		}
		*(bufp++) = c;
		incrp;
		c = nextc;
	}
	
	*bufp = '\0';
	ff_uint_t l;
	char *p = (char*)malloc((l = (bufp-buf))+1);
	dus_to_free(p);
	memcpy(p, buf, l+1);
	*__l = l;
	return p;
}

char static*
read_dec(ff_uint_t *__l) {
	char buf[128];
	char *bufp = buf;
	char c = nextc;
	while(c>='0' && c<='9') {
		if ((bufp-buf)>=128) {
			// buffer overflow
		}
		*(bufp++) = c;
		incrp;
		c = nextc;
	}

	*bufp = '\0';
	ff_uint_t l;
	char *p = (char*)malloc((l = (bufp-buf))+1);
	dus_to_free(p);
	memcpy(p, buf, l+1);
	*__l = l;
	return p;
}

char static*
read_str(ff_uint_t *__l) {
	char buf[1024];
	char *bufp = buf;
	char c = nextc;
	while(c != '"') {
		if ((bufp-buf)>=1024) {
			// buffer overflow
		}
		*(bufp++) = c;
		incrp;
		c = nextc;
	}

	*bufp = '\0';
	ff_uint_t l;
	char *p = (char*)malloc((l = (bufp-buf))+1);
	dus_to_free(p);
	memcpy(p, buf, l+1);
	*__l = l;
	return p;
}

dus_tokenp dus_peektok(void) {
	dus_tokenp ret;
	ff_dus_ulex(ret = ff_dus_nexttok());
	return ret;
}

#define BUFSIZE 20
dus_tokenp static buf[BUFSIZE];
dus_tokenp static* end = buf;

void ff_dus_ulex(dus_tokenp __p) {
	*(end++) = __p;
}

/*
	if any token issues occur this should be the first suspect

	after x tokens oldest one will be freed
*/
#define BACK 10
dus_tokenp static head = NULL;
dus_tokenp static next = NULL;
ff_uint_t static len = 0;

dus_tokenp ff_dus_lex(void) {
	if (end>buf)
		return *(--end);
	if (len>BACK && head != NULL) {
		len--;
		dus_tokenp bk = head;
		head = head->next;
		free(bk);
	}

_bk:
	sk_white_space();
	if (dus_at_eof()) {
		return NULL;
	}

	if (*dus_p == '#') {
		while(*dus_p != '\n') {
			if (dus_at_eof())
				return NULL;
			incrp;
		}
		incrp;
		goto _bk;
	}

	dus_tokenp tok;
	tok = (dus_tokenp)malloc(sizeof(struct dus_token));
	tok->p = NULL;
	tok->next = NULL;
	if (!head)
		head = tok;
	if (next != NULL)
		next->next = tok;
	next = tok;

	char c = nextc;
	if ((c>='a' && c<='z') || c == '_') {
		tok->sort = _dus_tok_ident;
		tok->p = read_ident(&tok->l);
	} else if (c>='0' && c<='9') {
		tok->sort = _dus_tok_dec;
		tok->p = read_dec(&tok->l);
	} else {
		switch(c) {
			case '$':
				tok->sort = _dus_tok_keywd;
				tok->val = _dus_dollar;
				incrp;
			break;
			case '"':
				incrp;
				tok->sort = _dus_tok_str;
				tok->p = read_str(&tok->l);
				incrp;
			break;
			case '=':
				tok->sort = _dus_tok_keywd;
				tok->val = _dus_eq;
				incrp;				
			break;
			default:
				printf("unknown %c\n", c);
		}
	}
	len++;
	return tok;
}

void ff_dus_lexer_cleanup(void) {
	dus_tokenp cur = head, tmp;
	while(cur != NULL) {
		tmp = cur->next;
		free(cur);
		cur = tmp;
	}
}
