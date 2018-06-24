# include "lexer.h"
# include "input.h"
# include "memalloc.h"
# include "../string.h"
# define is_space(__c) (__c == ' ' || __c == '\t')
void *tok_data;
ff_uint_t tok_sz;
ff_u8_t tok_val;
void static
read_ident(void) {
	char buf[128];
	char *p = buf;
	char c = getc();
	while((c>='a'&&c<='z') || (c>='0'&&c<='9')) {
		*(p++) = c;
		if (at_eof()) break;
		c = getc();
	}
	if (!at_eof())
		ugetc(c);

	*p = '\0';
	ff_uint_t l;

	l = p-buf;
	tok_data = mem_alloc(l+1);
	memcpy(tok_data, buf, l+1);
	tok_sz = l+1;
}

void static
read_str(void) {
	char buf[128];
	char *p = buf;
	char c;
	while((c = getc()) != '"')
		*(p++) = c;
	*p = '\0';
	ff_uint_t l;

	l = p-buf;
	tok_data = mem_alloc(l+1);
	memcpy(tok_data, buf, l+1);
	tok_sz = l+1;
}

ff_u8_t static buf = 0xff;

ff_u8_t nexttok(void) {
	if (buf<0xff) {
		ff_u8_t ret;

		ret = buf;
		buf = 0xff;
		return ret;
	}
	char c;
_again:
	if (at_eof())
		return _tok_eof;

	c = getc();
	if (is_space(c))
		goto _again;

	if (c>='a'&&c<='z') {
		ugetc(c);
		read_ident();
		return _tok_ident;
	} else if (c == '"') {
		read_str();		
		return _tok_str;
	} else {
		switch(c) {
			case '\n':
				return _tok_nl;
			case '*':
				tok_val = c;
				return _tok_keywd;
		}
	}
	return 0;
}

ff_u8_t peektok(void) {
	buf = nexttok();
	return buf;
}

char const* tokstr(ff_u8_t __tok) {
	switch(__tok) {
		case _tok_uk:
			return "unknown";
		case _tok_eof:
			return "eof";
		case _tok_ident:
			return "ident";
		case _tok_str:
			return "string";
		case _tok_keywd:
			return "keyword";
	}
	return "unknown";
}
