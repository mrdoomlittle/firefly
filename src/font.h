# ifndef __ffly__font__h
# define __ffly__font__h
# include "ffint.h"
#define _font_new()\
	ffly_font_new()
#define _font_init(...)\
	ffly_font_init(__VA_ARGS__)
#define _font_destroy(...)\
	ffly_font_destroy(__VA_ARGS__)
#define _font_face(...)\
	ffly_font_face(__VA_ARGS__)
struct font_driver {
	void(*init)(void);
	void(*done)(void);
	void(*face_load)(char const*);
	void(*glyph_load)(char);
	void(*scale)(ff_uint_t);
	void(*info)(ff_u8_t, long long);
};

typedef struct ffly_font {
	struct font_driver driver;

} *ffly_fontp;

enum {
	_font_driver_typo
};

enum {
	_face_width,
	_face_height
};

void ffly_font_info(ffly_fontp, ff_u8_t, long long);
ffly_fontp ffly_font_new(void);
void ffly_font_init(ffly_fontp, ff_u8_t);
void ffly_font_de_init(ffly_fontp);
void ffly_font_face(ffly_fontp, char const*);
ff_u8_t* ffly_font_char(ffly_fontp, char);
void ffly_font_destroy(ffly_fontp);
# endif /*__ffly__font__h*/
