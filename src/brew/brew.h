# ifndef __brew__h
# define __brew__h
# include <mdlint.h>
extern mdl_u8_t *p;
# define incrp p++
# define decrp p--
typedef struct bucket {
	struct bucket *next, *fd;
	mdl_u8_t sort:4;
	mdl_u8_t val;
	void *p;
	mdl_u16_t len;
} *bucketp;

typedef struct obj {
	struct obj *next;
} *objp;

enum {
	_colon,
	_keywd_cp
};

enum {
	_unknown,
	_ident,
	_chr,
	_keywd
};

bucketp nexttok();
void maybe_keyword(bucketp);
mdl_u8_t tokbuf_size();
mdl_u8_t expect_token(mdl_u8_t, mdl_u8_t);
bucketp peektok();
bucketp lex();
void ulex(bucketp);
void parse(bucketp*);
mdl_u8_t at_eof();
void lexer_cleanup();
void exec();
void oust(bucketp);
# endif /*__brew__h*/
