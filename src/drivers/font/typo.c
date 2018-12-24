# include "../../font.h"
# include "../../typo/raster.h"
# include "../../typo/raise.h"
# include "../../typo.h"
static struct ffly_typo_sheet sheet;
static struct typo_face face;
void static
init(void) {
	ffly_typo_init(&sheet, 0);
	ffly_traise_prime();
}

void static
done(void) {
	ffly_typo_done(&sheet);
}

void static
face_load(char const *__file) {
	ffly_typo_face(&sheet, __file, &face);


	ffly_typo_face_scale(&sheet, &face, 0);
}

void static
glyph_load(char __c) {
	ffly_typo_char_load(&sheet, &face, __c);
}

void static
scale(ff_uint_t __x) {
	ffly_typo_face_scale(&sheet, &face, __x);
}


#define FACE_WIDTH		0
#define FACE_HEIGHT		1
void static
info(ff_u8_t __what, long long __arg) {
	switch(__what) {
		case FACE_WIDTH:
			*(ff_uint_t*)__arg = ffly_typo_translate_to(sheet.f_width);
		break;
		case FACE_HEIGHT:
			*(ff_uint_t*)__arg = ffly_typo_translate_to(sheet.f_height);
		break;
	}
}

void ffly_font_typo(struct font_driver *__driver) {
	__driver->init = init;
	__driver->done = done;
	__driver->face_load = face_load;
	__driver->glyph_load = glyph_load;
	__driver->scale = scale;
	__driver->info = info;
}
