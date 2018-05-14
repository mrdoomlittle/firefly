# define __ffly_compiler_internal
# define __ffly_lexer
# include "lexer.h"
//# include "compiler.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "dep/mem_cpy.h"
# include "dep/str_len.h"
# include "system/string.h"
# include "memory/mem_alloc.h"
# include "memory/mem_realloc.h"
# include "memory/mem_free.h"

# define hdrsize sizeof(struct hdr)
typedef struct hdr {
	struct hdr *prev, *next;
	ff_uint_t bc;
	void *p;
} *hdrp;

void* ff_lexer_alloc(ff_lexerp __lexer, ff_uint_t __bc) {
	void *ret = __ffly_mem_alloc(hdrsize+__bc);
	hdrp h = (hdrp)ret;
	ret = (h->p = (void*)((ff_u8_t*)ret+hdrsize));
	hdrp *top = (hdrp*)&__lexer->top;
	h->next = *top;
	h->prev = NULL;
	if (*top != NULL)
		(*top)->prev = h;
	*top = h;
	h->bc = __bc;
	return ret;
}

void ff_lexer_free(ff_lexerp __lexer, void *__p) {
	void *p = (void*)((ff_u8_t*)__p-hdrsize);
	hdrp h = (hdrp)p;
	hdrp *top = (hdrp*)&__lexer->top;

	if (h == *top) {
		if ((*top = h->next) != NULL)
			(*top)->prev = NULL;
		goto _end;
	}
	if (h->next != NULL)
		h->next->prev = h->prev;
	if (h->prev != NULL)
		h->prev->next = h->next;
_end:														
	__ffly_mem_free(p);
}

# define PAGE_SHIFT 4
# define PAGE_SIZE (1<<PAGE_SHIFT)

ff_tokenp static *tokens = NULL;
ff_uint_t static t_off = 0;
ff_uint_t static t_page_c = 0;

ff_tokenp free_toks = NULL;

ff_tokenp ff_token_alloc() {
	if (free_toks != NULL) {
		ff_tokenp ret = free_toks;
		free_toks = free_toks->fd;
		return ret;
	}

	ff_uint_t page = t_off>>PAGE_SHIFT;
	ff_uint_t pg_off;
	if (!tokens) {
		tokens = __ffly_mem_alloc(sizeof(ff_tokenp));
		t_page_c = 1;
	} else {
		if (page>t_page_c-1)
			tokens = (ff_tokenp*)__ffly_mem_realloc(tokens, (++t_page_c)*sizeof(ff_tokenp));
		else
			goto _sk;
	}
	*(tokens+page) = (ff_tokenp)__ffly_mem_alloc(PAGE_SIZE*sizeof(struct token));
_sk:
	pg_off = ((t_off++)-(page*PAGE_SIZE));
	ff_tokenp ret = (*(tokens+page))+pg_off;
	ret->page = page;
	ret->pg_off = pg_off;
	return ret;
}

void ff_token_free(ff_tokenp __tok) {
	if (__tok->pg_off+(__tok->page*PAGE_SIZE) == t_off-1) {
		t_off--;
		ff_uint_t page = t_off>>PAGE_SHIFT;
		if (page < t_page_c-1 && t_page_c>1) {
			__ffly_mem_free(*(tokens+(page+1)));
			tokens = (ff_tokenp*)__ffly_mem_realloc(tokens, (--t_page_c)*sizeof(ff_tokenp));
		}
		return;
	} else {
		__tok->fd = free_toks;
		free_toks = __tok;
	}
}

void ff_token_cleanup(ff_tokenp __tok) {
	ff_u8_t kind = __tok->kind;
	if (kind == _tok_ident) {

	} else if (kind == _tok_keywd) {

	} else if (kind == _tok_no) {

	} else if (kind == _tok_str) {

	} else if (kind == _tok_newline) {

	} else if (kind == _tok_chr) {

	}
}

void ff_token_destroy(ff_tokenp __tok) {
	ff_token_cleanup(__tok);
	ff_token_free(__tok);
}

# define incp \
	(__lexer)->p++
# define decp \
	(__lexer)->p--

ff_bool_t
at_eof(ff_lexerp __lexer) {
	return(__lexer->p>=__lexer->end);
}

char static
fetchc(ff_lexerp __lexer) {
	if (at_eof(__lexer)) return '\0';
	return *__lexer->p;
}

char static
nextc(ff_lexerp __lexer) {
	return *(__lexer->p+1);
}

ff_bool_t static
is_next(ff_lexerp __lexer, char __c) {
	return (*(__lexer->p+1) == __c);
}

ff_bool_t static
is_prev(ff_lexerp __lexer, char __c) {
	return (*(__lexer->p-1) == __c);
}

ff_bool_t static
is_space(ff_lexerp __lexer) {
	char c;
	if ((c = fetchc(__lexer)) == '\0') {
		// something may have gone terribly wong
	}

	ff_i8_t static comment = 0;
	if (comment>0) {
		if (c == '/' && is_prev(__lexer, '*'))
			comment = 0;
		return 1;
	} else if (comment<0) {
		if (c == '\n')
			comment = 0;
		return 1;
	}

	if (c == '/' && is_next(__lexer, '*')) {
		comment = 1;
		return 1;
	} else if (c == '/' && is_next(__lexer, '/')) {
		comment = -1;
		return 1;
	}
	
	return (c == ' ' || c == '\t');
}

ff_bool_t static
at_nl(ff_lexerp __lexer) {
	return (fetchc(__lexer) == '\n');
}

void static
escape_chr(char *__c, char *__p) {
	switch(*__p) {
		case 'n':
			*__c = '\n';
		break;
		case 't':
			*__c = '\t';
		break;
	}
}

char static*
read_ident(ff_lexerp __lexer) {
	char *p = (char*)__lexer->p;
	char c = *p;
	while((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || (c >= '0' && c <= '9')) {
		ffly_buff_put(&__lexer->sbuf, p++);
		ffly_buff_incr(&__lexer->sbuf);
		c = *p;
	}

	char end = '\0';
	ffly_buff_put(&__lexer->sbuf, &end);

	ff_uint_t l = ffly_buff_off(&__lexer->sbuf);
	char *s = (char*)ff_lexer_alloc(__lexer, l+1);
	ffly_mem_cpy(s, ffly_buff_begin(&__lexer->sbuf), l+1);

	__lexer->p = (ff_u8_t*)p;
	ffly_buff_off_reset(&__lexer->sbuf);
	return s;
}

char static* 
read_no(ff_lexerp __lexer, ff_u8_t *__is_hex, ff_u8_t *__is_float) {
	char *p = (char*)__lexer->p;
	char c = *p;
	while((c >= '0' && c <= '9') || c == '.' || c == '-' || ffly_tolow(c) == 'x' || (ffly_tolow(c) >= 'a' && ffly_tolow(c) <= 'f')) {
		if (ffly_tolow(*p) == 'x') *__is_hex = 1;
		if (*p == '.') *__is_float = 1;
		ffly_buff_put(&__lexer->sbuf, p++);
		ffly_buff_incr(&__lexer->sbuf);
		c = *p;
	}

	char end = '\0';
	ffly_buff_put(&__lexer->sbuf, &end);

	ff_uint_t l = ffly_buff_off(&__lexer->sbuf);
	char *s = (char*)ff_lexer_alloc(__lexer, l+1);
	ffly_mem_cpy(s, ffly_buff_begin(&__lexer->sbuf), l+1);

	__lexer->p = (ff_u8_t*)p;
	ffly_buff_off_reset(&__lexer->sbuf);
	return s;
}

char static* 
read_str(ff_lexerp __lexer, ff_uint_t *__l) {
	char *p = (char*)__lexer->p;
	while(*p != '"') {
		if (*p == 0x5c) {
			p++;
			char c;
			escape_chr(&c, p);
			p++;
			ffly_buff_put(&__lexer->sbuf, &c);
			ffly_buff_incr(&__lexer->sbuf);
			continue;
		}

		ffly_buff_put(&__lexer->sbuf, p++);
		ffly_buff_incr(&__lexer->sbuf);
	}

	char end = '\0';
	ffly_buff_put(&__lexer->sbuf, &end);

	ff_uint_t l = ffly_buff_off(&__lexer->sbuf);
	char *s = (char*)ff_lexer_alloc(__lexer, l+1);
	ffly_mem_cpy(s, ffly_buff_begin(&__lexer->sbuf), l+1);

	__lexer->p = (ff_u8_t*)p;
	ffly_buff_off_reset(&__lexer->sbuf);
	*__l = l;
	return s;
}

void static
mk_keywd(struct token *__tok, ff_u8_t __id) {
	__tok->kind = _tok_keywd;
	__tok->id = __id;
}

char static*
read_chr(ff_lexerp __lexer) {
	return (char*)(__lexer->p++);
}

# define register_line(__lexer) \
	(*__lexer->line)++; \
	(*__lexer->lo) = __lexer->p-__lexer->bed;

struct token static*
read_token(ff_lexerp __lexer) {
	struct token *tok;
	if (!(tok = ff_token_alloc())) {
		// memory allocation failure
	}

	tok->p = NULL;
	if (at_nl(__lexer)) {
		register_line(__lexer);
		incp;
		tok->kind = _tok_newline;  
		return tok;
	}

	char c;
	switch(c = fetchc(__lexer)) {
		case '[':
			mk_keywd(tok, _l_bracket);
			incp;
		break;
		case ']':
			mk_keywd(tok, _r_bracket);
			incp;
		break;
		case '&':
			if (is_next(__lexer, '&')) {
				mk_keywd(tok, _and);
				incp;
			} else
				mk_keywd(tok, _ampersand);
			incp;
		break;
		case '\x27':
			incp;
			*tok = (struct token) {
				.kind = _tok_chr,
				.p = (void*)read_chr(__lexer) 
			};
			incp;
		break;
		case '*':
			mk_keywd(tok, _astrisk);
			incp;
		break;
		case '+':
			if (is_next(__lexer, '+')) {
				mk_keywd(tok, _incr);
				incp;
			} else
				mk_keywd(tok, _plus);
			incp;
		break;
		case '"':
			incp;
			tok->kind = _tok_str;
			tok->p = (void*)read_str(__lexer, &tok->l);
			incp;
		break;
		case '%':
			mk_keywd(tok, _percent);
			incp;
		break;
		case ':':
			mk_keywd(tok, _colon);
			incp;
		break;
		case '.':
			if (is_next(__lexer, '.')) {
				incp;
				if (is_next(__lexer, '.')) {
					mk_keywd(tok, _ellipsis);
					incp;
				}
			} else
				mk_keywd(tok, _period);
			incp;
		break;
		case '-':
			if (nextc(__lexer) >= '0' && nextc(__lexer) <= '9') goto _no;
			if (is_next(__lexer, '-')) {
				mk_keywd(tok, _decr);
				incp;
			} else if (is_next(__lexer, '>')) {
				mk_keywd(tok, _r_arrow);
				incp;
			} else
				mk_keywd(tok, _minus);
			incp;
		break;
		case ',':
			mk_keywd(tok, _comma);
			incp;
		break;
		case '<':
			if (is_next(__lexer, '-')) {
				mk_keywd(tok, _l_arrow);
				incp;
			} else
				mk_keywd(tok, _lt);
			incp;
		break;
		case '>':
			mk_keywd(tok, _gt);
			incp;
		break;
		case '!':
			if (is_next(__lexer, '=')) {
				mk_keywd(tok, _neq);
				incp;
			}
			incp;
		break;
		case ';':
			mk_keywd(tok, _semicolon);
			incp;
		break;
		case '=':
			if (is_next(__lexer, '=')) {
				mk_keywd(tok, _eqeq);
				incp;
			} else
				mk_keywd(tok, _eq);
			incp;
		break;
		case '(':
			mk_keywd(tok, _l_paren);
			incp;
		break;
		case ')':
			mk_keywd(tok, _r_paren);
			incp;
		break;
		case '{':
			mk_keywd(tok, _l_brace);
			incp;
		break;
		case '}':
			mk_keywd(tok, _r_brace);
			incp;
		break;
		default:
		if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_') {
			*tok = (struct token) {
				.kind = _tok_ident,
				.p = (void*)read_ident(__lexer)
			};
		} else if (c >= '0' && c <= '9') goto _no;
		else {
			incp;
			tok->kind = _tok_null;
		}
		break;
		_no:
		{
			ff_u8_t is_float = 0, is_hex = 0;
			*tok = (struct token) {
				.kind = _tok_no,
				.p = (void*)read_no(__lexer, &is_hex, &is_float)
			};
			tok->is_float = is_float;
			tok->is_hex = is_hex;
		}
	}
	
	tok->line = *__lexer->line;
	tok->off = (__lexer->p-__lexer->bed)-1;
	tok->lo = *__lexer->lo;
	return tok;
}

void ffly_ulex(ff_lexerp __lexer, struct token *__tok) {
	if (!__tok) return;
	ffly_buff_put(&__lexer->tokbuf, (void*)&__tok);
	ffly_buff_incr(&__lexer->tokbuf);
}

void ffly_lexer_init(ff_lexerp __lexer) {
	ffly_buff_init(&__lexer->tokbuf, 100, sizeof(struct token*));
	ffly_buff_init(&__lexer->sbuf, 246, 1);
	__lexer->top = NULL;
}

void ffly_lexer_cleanup(ff_lexerp __lexer) {
	ffly_buff_de_init(&__lexer->tokbuf);
	ffly_buff_de_init(&__lexer->sbuf);

	hdrp cur = (hdrp)__lexer->top, bk;
	while(cur != NULL) {
		bk = cur;
		cur = cur->next;
		ff_lexer_free(__lexer, bk->p);
	}

	if (tokens != NULL) {
		ff_tokenp *page = tokens;
		while(page != tokens+t_page_c)
			__ffly_mem_free(*(page++));
		__ffly_mem_free(tokens);
	}
}

struct token*
ffly_lex(ff_lexerp __lexer, ff_err_t *__err) {
	if (ffly_buff_off(&__lexer->tokbuf)>0) {
		ffly_buff_decr(&__lexer->tokbuf);
		struct token *tok;
		ffly_buff_get(&__lexer->tokbuf, (void*)&tok);
		return tok;
	}

	while(is_space(__lexer) && !at_eof(__lexer)) {
		if (at_nl(__lexer)) {
			register_line(__lexer); 
		}
		incp;
	}

	if (at_eof(__lexer)) return NULL;

	struct token *tok;
	while(!(tok = read_token(__lexer)) && !at_eof(__lexer));
	return tok;
}
/*
ff_err_t ffmain(int __argc, char const *___argv[]) {
	char const *s = "hello 128 u8_t test;";

	ff_uint_t line, lo;
	struct ff_lexer lexer = {
		.p = s,
		.end = s+ffly_str_len(s)-1,
		.bed = s,
		.line = &line,
		.lo = &lo
	};

	ffly_lexer_init(&lexer);
	ff_err_t err;
	struct token *tok;
	while(!at_eof(&lexer)) {
		tok = ffly_lex(&lexer, &err);
		ffly_printf("%s\n", tok->p);
	}

	ffly_lexer_cleanup(&lexer);
}*/
