# include "text.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../dep/str_dup.h"
# include "../system/io.h"
ffly_ui_textp ffly_ui_text_creat(char const *__s) {
	ffly_ui_textp tx;

	tx = (ffly_ui_textp)__ffly_mem_alloc(sizeof(struct ffly_ui_text));
	tx->s = ffly_str_dup(__s);
	return tx;
}

void ffly_ui_text_font(ffly_ui_textp __text, ffly_fontp __font) {
	__text->font = __font;
}
void ffly_ui_text_draw(ffly_ui_textp __text, ff_u8_t *__dst,
	ff_uint_t __x, ff_uint_t __y,
	ff_uint_t __width, ff_uint_t __height)
{
	ff_u8_t *chr;
	char const *p;
	char c;

	p = __text->s;
	ff_uint_t fw, fh;
	ffly_font_info(__text->font, _face_width, &fw);
	ffly_font_info(__text->font, _face_height, &fh);

	if (!fw || !fh) {
		ffly_fprintf(ffly_err, "error: can't draw text, width or height is zero, %ux%u\n", fw, fh);
		return;
	}

	ffly_fprintf(ffly_log, "face size: %ux%u\n", fw, fh);

	ff_uint_t xoff, yoff;

	xoff = 0;
	yoff = 0;

	while((c = *(p++)) != '\0') {
		ff_u8_t *bm;
		ffly_fprintf(ffly_log, "drawing char %c\n", c);

		bm = ffly_font_char(__text->font, c);
		ff_uint_t x, y;
		y = 0;
		while(y != fh) {
			x = 0;
			while(x != fw) {
				ff_u8_t *p, *dst;
				p = bm+x+(y*fw);

				dst = __dst+(((__x+x+xoff)+((__y+y+yoff)*__width))*4);
				// for testing

				if (*p > 0)
					*(ff_u32_t*)dst = 0xff|0xff<<24;
				x++;
			}
			y++;
		}

		xoff+=fw;
	}
}

void ffly_ui_text_destroy(ffly_ui_textp __text) {
	__ffly_mem_free(__text);
}
