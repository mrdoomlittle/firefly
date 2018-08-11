# include "lexer.h"
# include "memalloc.h"
# include "../dep/mem_cpy.h"
# include "../system/io.h"
# define is_space(__c) \
	((__c) == ' ' || (__c) == '\t' || (__c) == '\n')
ff_uint_t tok_sz;
ff_u8_t tok_val;
void *tok_data;

void static
read_ident(ff_hsp __hs) {
	char buf[128];
	char *p;

	p = buf;
	char c;
	c = ff_hs_getc(__hs);
	while((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_') {
		*(p++) = c;
		c = ff_hs_getc(__hs);
	}

	ff_hs_ugetc(__hs, c);

	*p = '\0';
	tok_sz = (p-buf)+1;
	tok_data = ff_hs_mem_alloc(tok_sz);
	ffly_mem_cpy(tok_data, buf, tok_sz);
}

void static
read_no(ff_hsp __hs) {
	char buf[128];
	char *p;

	p = buf;
	char c;
	c = ff_hs_getc(__hs);
	while(c >= '0' && c <= '9') {
		*(p++) = c;
		c = ff_hs_getc(__hs);
	}

	ff_hs_ugetc(__hs, c);
	*p = '\0';
	tok_sz = (p-buf)+1;
	tok_data = ff_hs_mem_alloc(tok_sz);
	ffly_mem_cpy(tok_data, buf, tok_sz);
}

struct token {
	ff_u8_t kind, val;
	ff_uint_t sz;
	void *data;
};

static char const *empty_str = "nothing";
static struct token tokbuf[20];
static struct token *btok = tokbuf;
ff_u8_t nexttok(ff_hsp __hs) {
	if (btok>tokbuf) {
		struct token *t;
		t = --btok;
		tok_sz = t->sz;
		tok_val = t->val;
		tok_data = t->data;
		return t->kind;
	}

	tok_data = empty_str;
	char c;
_skc:
	if (ff_hs_at_eof(__hs))
		return _null;
	c = ff_hs_getc(__hs);

	if (c == '/') {
		c = ff_hs_getc(__hs);
		if (c == '/') {
			while((c = ff_hs_getc(__hs)) != '\n') {
				if (ff_hs_at_eof(__hs))
					return _null;
			}
		} else
			ff_hs_ugetc(__hs, c);
	}

	if (is_space(c)) {
		goto _skc;
	}

	if (c>='a'&&c<='z') {
		ff_hs_ugetc(__hs, c);
		read_ident(__hs);
		return _tok_ident;
	} else if (c>='0'&&c<='9') {
		ff_hs_ugetc(__hs, c);
		read_no(__hs);
		return _tok_no;
	} else {
		switch(c) {
			case '=':
				tok_val = _eq;
			return _tok_keywd;
			case '.':
				tok_val = _period;
			return _tok_keywd;
			case ',':
				tok_val = _comma;
			return _tok_keywd;
			case '[':
				tok_val = _l_bracket;
			return _tok_keywd;
			case ']':
				tok_val = _r_bracket;
			return _tok_keywd;
			case ';':
				tok_val = _semicolon;
			return _tok_keywd;
			case '{':
				tok_val = _l_brace;
			return _tok_keywd;
			case '}':
				tok_val = _r_brace;
			return _tok_keywd;
			case '#':
				tok_val = _pound;
			return _tok_keywd;
		}
		ffly_printf("unknown.\n");
	}
	return _null;
}

void rtok(ff_u8_t __tok) {
	struct token *t;
	t = btok++;
	t->kind = __tok;
	t->val = tok_val;
	t->sz = tok_sz;
	t->data = tok_data;
}

ff_u8_t peektok(ff_hsp __hs) {
	ff_u8_t tok;
	rtok(tok = nexttok(__hs));
	return tok;
}
