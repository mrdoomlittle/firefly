# ifndef __ffly__ui__text__h
# define __ffly__ui__text__h
# include "../ffint.h"
# include "../font.h"
typedef struct ffly_ui_text {
	char const *s;
	ffly_fontp font;
} *ffly_ui_textp;

ffly_ui_textp ffly_ui_text_creat(char const*);
void ffly_ui_text_font(ffly_ui_textp, ffly_fontp);
void ffly_ui_text_draw(ffly_ui_textp, ff_u8_t*, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t);
void ffly_ui_text_destroy(ffly_ui_textp);
# endif /*__ffly__ui__text__h*/
