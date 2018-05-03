# ifndef __brew__h
# define __brew__h
# include "../ffint.h"
extern ff_u8_t *p;
# define incrp p++
# define decrp p--
typedef struct bucket {
	struct bucket *next, *fd;
	void *src, *dst;
	ff_u8_t sort:4;
	ff_u8_t val;
	void *p;
	ff_u16_t len;
} *bucketp;

enum {
	_op_cp,
	_op_exit,
	_op_jump,
	_op_echo,
	_op_end,
	_op_shell
};

typedef struct obj {
	struct obj *next;
	ff_u8_t opcode;
	void *src, *dst, *p;
	struct obj *to;
} *objp;

enum {
	_colon,
	_comma,
	_circumflex,
	_semicolon,
	_period,
	_keywd_entry,
	_keywd_cp,
	_keywd_exit,
	_keywd_end,
	_keywd_echo
};

enum {
	_label,
	_cp,
	_exit,
	_end,
	_jump,
	_shell,
	_echo
};

enum {
	_unknown,
	_ident,
	_chr,
	_keywd,
	_str
};

ff_u8_t is_keywd(bucketp, ff_u8_t);
bucketp nexttok();
void maybe_keyword(bucketp);
ff_u8_t tokbuf_size();
ff_u8_t expect_token(ff_u8_t, ff_u8_t);
bucketp peektok();
bucketp lex();
void ulex(bucketp);
void parse(bucketp*);
ff_u8_t at_eof();
void lexer_cleanup();
void brew_exec(objp);
void gen(bucketp, objp*);
void oust(bucketp, ff_u8_t);
void to_free(void*);
# endif /*__brew__h*/
