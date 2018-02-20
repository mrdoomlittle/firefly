# ifndef __brew__h
# define __brew__h
# include <mdlint.h>
extern mdl_u8_t *p;
# define incrp p++
# define decrp p--
typedef struct bucket {
	struct bucket *next, *fd;

	void *src, *dst;
	mdl_u8_t sort:4;
	mdl_u8_t val;
	void *p;
	mdl_u16_t len;
} *bucketp;

enum {
	_op_cp,
	_op_exit
};

typedef struct obj {
	struct obj *next;
	mdl_u8_t opcode;
	void *src, *dst;
} *objp;

enum {
	_colon,
	_keywd_cp,
	_keywd_exit
};

enum {
	_label,
	_cp,
	_exit
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
void gen(bucketp);
void oust(bucketp, mdl_u8_t);
void to_free(void*);
# endif /*__brew__h*/
