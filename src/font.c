# include "font.h"
# include "drivers/font.h"
# include "typo/raster.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
static void(*load_driver[])(struct font_driver*) = {
	ffly_font_typo
};

void ffly_font_info(ffly_fontp __font, ff_u8_t __what, long long __arg) {
	__font->driver.info(__what, __arg);
}

ffly_fontp ffly_font_new(void) {
	ffly_fontp ft;
	ft = (ffly_fontp)__ffly_mem_alloc(sizeof(struct ffly_font));
}

void ffly_font_destroy(ffly_fontp __font) {
	ffly_font_de_init(__font);
	__ffly_mem_free(__font);
}

void ffly_font_init(ffly_fontp __font, ff_u8_t __driver) {
	load_driver[__driver](&__font->driver);
	__font->driver.init();
}

void ffly_font_de_init(ffly_fontp __font) {
	__font->driver.done();
}

void ffly_font_face(ffly_fontp __font, char const *__file) {
	__font->driver.face_load(__file);
}

ff_u8_t* ffly_font_char(ffly_fontp __font, char __c) {
	__font->driver.glyph_load(__c);
	return typo_raster_bm; // for now
}

