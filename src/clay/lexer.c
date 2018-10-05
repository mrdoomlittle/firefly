# include "lexer.h"
# include "input.h"
# include "memalloc.h"
# include "../string.h"
# include "../ffly_def.h"
# define is_space(__c) (__c == ' '|| __c == '\t' || __c == '\n')
ff_uint_t clay_tok_sz;
ff_u8_t clay_tok_val;
void *clay_tok_data;

void static
read_str(void) {
	char buf[128];
	char *bp;
	char c;

	bp = buf;
	while((c = clay_getc()) != '"') {
		*(bp++) = c;
	}

	*bp = '\0';
	clay_tok_sz = (bp-buf)+1;
	clay_tok_data = clay_mem_alloc(clay_tok_sz);
	memcpy(clay_tok_data, buf, clay_tok_sz);
}

void static
read_no(void) {
	char buf[128];
	char *bp;
	char c;

	bp = buf;
	c = clay_getc();
	while(c >= '0' && c <= '9') {
		*(bp++) = c;
		c = clay_getc();
	}
	clay_ugetc(c);

	*bp = '\0';
	clay_tok_sz = (bp-buf)+1;
	clay_tok_data = clay_mem_alloc(clay_tok_sz);
	memcpy(clay_tok_data, buf, clay_tok_sz);
}

void static
read_ident(void) {
	char buf[128];
	char *bp;
	char c;

	bp = buf;
	c = clay_getc();
	while((c >= 'a' && c <= 'z') || c == '_' || (c>='0'&& c<='9')) {
		*(bp++) = c;
		c = clay_getc();
	}
	clay_ugetc(c);

	*bp = '\0';
	clay_tok_sz = (bp-buf)+1;
	clay_tok_data = clay_mem_alloc(clay_tok_sz);
	memcpy(clay_tok_data, buf, clay_tok_sz);
}

struct token {
	ff_u8_t kind, val;
	ff_uint_t sz;
	void *data;
};

static struct token tokbuf[20];
static struct token *btok = tokbuf;

ff_u8_t clay_nexttok(ffly_clayp __clay) {
	if (btok>tokbuf) {
		struct token *t;
		t = --btok;
		clay_tok_sz = t->sz;
		clay_tok_val = t->val;
		clay_tok_data = t->data;
		return t->kind;
	}

	char c;
_again:
	if (clay_at_eof())
		return ~0;
	c = clay_getc();

	if (is_space(c))
		goto _again;
	if (c>='0'&& c<='9') {
		clay_ugetc(c);
		read_no();
		return _clay_tok_no;
	} else if ((c>='a' && c<='z') || c == '_' || (c>='0'&& c<='9')) {
		clay_ugetc(c);
		read_ident();
		return _clay_tok_ident;
	} else {
		switch(c) {
			case '~':
				clay_tok_val = _clay_tilde;
				return _clay_tok_keywd;
			case '"':
				read_str();
				return _clay_tok_str;
			case '{':
				clay_tok_val = _clay_l_brace;
				return _clay_tok_keywd;
			case '}':
				clay_tok_val = _clay_r_brace;
				return _clay_tok_keywd;
		}
	}
	return ~0;	
}

void clay_rtok(ff_u8_t __tok) {
	struct token *t;
	t = btok++;
	t->kind = __tok;
	t->val = clay_tok_val;
	t->sz = clay_tok_sz;
	t->data = clay_tok_data;
}

ff_u8_t clay_peektok(ffly_clayp __clay) {
	ff_u8_t tok;
	clay_rtok(tok = clay_nexttok(__clay));
	return tok;
}
