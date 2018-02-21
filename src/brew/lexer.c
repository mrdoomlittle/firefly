# include "brew.h"
# include "../stdio.h"
# include "../malloc.h"
# include "../string.h"
# define is_no(__c) (__c >= '0' && __c <= '9')
# define is_ident(__c) (__c >= 'a' && __c <= 'z')
char static nextc() {
	return *p;
}

mdl_u8_t static is_space(char __c) {
	return (__c == ' ' | __c == '\t' | __c == '\n');
}

void static sk_white_space() {
	while(is_space(nextc()) && !at_eof()) incrp;	
}

# define BUFSIZE 20

bucketp static buf[BUFSIZE];
bucketp static* end = buf;

mdl_u8_t tokbuf_size() {
	return end-buf;
}

void ulex(bucketp __p) {
	if (end >= buf+BUFSIZE) {
		fprintf(stderr, "lexer buffer full.\n");
		return;
	}

	*(end++) = __p;
}

mdl_u8_t expect_token(mdl_u8_t __sort, mdl_u8_t __val) {
	bucketp tok = nexttok();
	mdl_u8_t res = (tok->sort == __sort && tok->val == __val);
	if (!res) {
		fprintf(stdout, "got %u expected: %u, sort: %u\n", tok->val, __val, tok->sort);
	}

	return res;
}

char* read_ident(mdl_u16_t *__len) {
	char buf[1024];
	char *bufp = buf;
	while(nextc() >= 'a' && nextc() <= 'z') {
		*(bufp++) = *p;
		incrp;
	}
	*bufp = '\0';
	mdl_u16_t len;
	char *p = (char*)malloc((len = (bufp-buf))+1);
	to_free(p);
	memcpy(p, buf, len+1);		
	*__len = len;
	return p;
}

/*
	tokens will be freed from head down if no. of is grater then	
*/
# define BACK 10
bucketp static head = NULL;
bucketp static next = NULL;
mdl_uint_t static len = 0;

bucketp lex() {
	if (end > buf)
		return *(--end);
	if (len > BACK && head != NULL) {
		len--;
		bucketp bk = head;
		head = head->next;
		free(bk);
	}

	if (at_eof()) return NULL;

	bucketp ret;
	if ((ret = (bucketp)malloc(sizeof(struct bucket))) == NULL) {
		// err
	}

	ret->next = NULL;
	ret->fd = NULL;
	sk_white_space();
	if (!head)
		head = ret;

	char c = nextc();
	if (is_no(c)) {
		// number
		incrp;
	} else if (is_ident(c)) {
		ret->sort = _ident;
		ret->p = read_ident(&ret->len);
	} else {
		switch(c) {
			case ':':
				ret->sort = _keywd;
				ret->val = _colon;
				incrp;
			break;
			case ',':
				ret->sort = _keywd;
				ret->val = _comma;
				incrp;
			break;
			case '^':
				ret->sort = _keywd;
				ret->val = _circumflex;
				incrp;
			break;
			case ';':
				ret->sort = _keywd;
				ret->val = semicolon;
				incrp;
			break;
			default:
				ret->sort = _unknown;
				fprintf(stderr, "unknown token, got: '%c'\n", c);
				incrp;	
		}
	}

	if (!next)
		next = ret;
	else {
		next->next = ret;
		next = ret;
	}

	len++;
	return ret;
}

void lexer_cleanup() {
	bucketp cur = head, tmp;
	while(cur != NULL) {
		tmp = cur->next;
		free(cur);
		cur = tmp;
	}
}

