# ifndef __brew__h
# define __brew__h
# include <mdlint.h>
extern mdl_u8_t *p;
# define incrp p++
# define decrp p--
typedef struct bucket {
	struct bucket *next;
	mdl_u8_t sort, val;
	void *p;
	mdl_u16_t len;
} *bucketp;

enum {
	_colon
};

enum {
	_unknown,
	_ident,
	_chr
};

mdl_u8_t tokbuf_size();
mdl_u8_t expect_token(mdl_u8_t, mdl_u8_t);
bucketp peek_token();
bucketp lex();
void ulex(bucketp);
void parse(bucketp*);
mdl_u8_t at_eof();
void lexer_cleanup();
# endif /*__brew__h*/
