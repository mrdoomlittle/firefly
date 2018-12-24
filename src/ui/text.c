# include "text.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../dep/str_dup.h"
# include "../system/io.h"
# include "../font.h"
ffly_ui_textp ffly_ui_text_creat(char const *__s) {
	ffly_ui_textp tx;

	tx = (ffly_ui_textp)__ffly_mem_alloc(sizeof(struct ffly_ui_text));
	tx->s = ffly_str_dup(__s);
	return tx;
}

void static font_info(long long __f, ff_u8_t __what, long long __arg) {
	ffly_font_info((ffly_fontp)__f, __what, __arg);
}

void static font_char(long long __f, ff_u8_t __id) {
	ffly_font_char((ffly_fontp)__f, __id);
}

# include "../bitfont.h"
void static bitfont_info(long long __f, ff_u8_t __what, long long __arg) {
	ffly_bitfont_info(__what, __arg);
}

void static bitfont_char(long long __f, ff_u8_t __id) {
	ffly_bitfont_char(__id);
}

void ffly_ui_text_font(ffly_ui_textp __text, long long __font, ff_u8_t __type) {
	__text->font = __font;
	switch(__type) {
		case _uitext_default:
			__text->font_info = font_info;
			__text->font_char = font_char;
		break;
		case _uitext_bitfont:
			__text->font_info = bitfont_info;
			__text->font_char = bitfont_char;
		break;
	}
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
	__text->font_info(__text->font, _face_width, &fw);
	__text->font_info(__text->font, _face_height, &fh);

	if (!fw || !fh) {
		ffly_fprintf(ffly_err, "error: can't draw text, width or height is zero, %ux%u\n", fw, fh);
		return;
	}

	ffly_fprintf(ffly_log, "face size: %ux%u\n", fw, fh);

	ff_uint_t xoff, yoff;

	xoff = 0;
	yoff = 0;

	while((c = *(p++)) != '\0') {
		if (c == '\n') {
			yoff+=fh;
			xoff = 0;
			continue;
		}
		ff_u8_t *bm;
		ffly_fprintf(ffly_log, "drawing char %c\n", c);
		bm = __text->font_char(__text->font, c);
		ff_uint_t x, y;
		y = 0;
		while(y != fh) {
			x = 0;
			while(x != fw) {
				ff_u8_t *p, *dst;
				p = bm+x+(y*fw);

				dst = __dst+(((__x+x+xoff)+((__y+y+yoff)*__width))*4);
				// for testing

				if (*p>0)
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
