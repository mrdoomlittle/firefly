# ifndef __ffly__pp__h
# define __ffly__pp__h
# include <mdlint.h>
# include "../ffly_def.h"
mdl_u8_t extern *p;
mdl_u8_t extern *end;
int extern src;
int extern dst;

typedef struct bucket {
	mdl_u8_t sort, val;
	mdl_u8_t *p;
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

mdl_u8_t at_eof();
bucketp lex();
void oust(mdl_u8_t);
void prossess();
void flushbuf();
void prep();
void finish();
void ulex(bucketp);
void sk_space();
void define(char*, mdl_u64_t, mdl_u8_t*, mdl_u8_t*);
# endif /*__ffly__pp__h*/
