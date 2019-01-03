# ifndef __ffly__ui__text__h
# define __ffly__ui__text__h
# include "../ffint.h"

#define _ui_text_creat		ffly_ui_text_creat
#define _ui_text_font		ffly_ui_text_font
#define _ui_text_destroy	ffly_ui_text_destroy
#define _ui_text_draw		ffly_ui_text_draw

enum {
	_uitext_default,
	_uitext_bitfont
};
typedef struct ffly_ui_text {
	char const *s;
	long long font;

	void(*font_info)(long long, ff_u8_t, long long);
	ff_u8_t*(*font_char)(long long, ff_u8_t);
} *ffly_ui_textp;

ffly_ui_textp ffly_ui_text_creat(char const*);
void ffly_ui_text_font(ffly_ui_textp, long long, ff_u8_t);
void ffly_ui_text_draw(ffly_ui_textp, ff_u8_t*, ff_uint_t, ff_uint_t, ff_uint_t, ff_uint_t);
void ffly_ui_text_destroy(ffly_ui_textp);
# endif /*__ffly__ui__text__h*/
