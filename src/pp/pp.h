# ifndef __ffly__pp__h
# define __ffly__pp__h
# include "../ffint.h"
# include "../ffly_def.h"
ff_u8_t extern *p;
ff_u8_t extern *end;
int extern src;
int extern dst;

typedef struct bucket {
	ff_u8_t sort, val;
	ff_u8_t *p;
} *bucketp;

enum {
	_percent,
	_ident,
	_chr
};

enum {
	_define,
	_ifndef,
	_ifdef,
	_endif
};

ff_u8_t at_eof();
bucketp lex();
void oust(ff_u8_t);
void prossess();
void flushbuf();
void prep();
void finish();
void ulex(bucketp);
void sk_space();
void define(char*, ff_u64_t, ff_u8_t*, ff_u8_t*);
# endif /*__ffly__pp__h*/
