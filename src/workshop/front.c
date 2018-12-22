# include "front.h"
# include "font_forge.h"
# include "../workshop.h"
ff_u8_t static *tex0, *tex1;
# include "../system/io.h"
# include "../ui/text.h"
# include "../font.h"
# include "../malloc.h"
# include "../ui/btn.h"
# include "../ui/window.h"
# include "../dep/mem_set.h"
static ffly_gui_btnp fontforge;
static struct ffly_gui_window window;

enum {
	_bt_fontforge
};

ff_u8_t static flags = 0xff;
#define CTFF 0x01
void static
bt_press(ffly_gui_btnp __btn, void *__arg) {
	ffly_printf("button press.\n");
	if (__btn->id == _bt_fontforge) {
		ffly_printf("fontforge.\n");
		flags ^= CTFF;
	}
}

void static
bt_release(ffly_gui_btnp __btn, void *__arg) {
	ffly_printf("button, release.\n");
}

void static bt_hover(ffly_gui_btnp __btn, void *__arg) {
	ffly_printf("button hover.\n");
}

void static init(void) {
	tex0 = (ff_u8_t*)malloc(144*23*4);
	ffly_mem_set(tex0, 0, 144*23*4);

	ffly_gui_btnp btn;
	btn = _ui_btn_creat(tex0, 144, 23, FFLY_UI_BT_HOG, 0, 0, workshop.get, NULL);
	btn->press = bt_press;
	btn->hover = bt_hover;
	btn->release = bt_release;
	btn->id = _bt_fontforge;
	fontforge = btn;
	_ui_btn_sched(btn);
	_ui_btn_enable(btn);

	_plt_write(&btn->texture, tex0, 144, 23, 0, 0);

	_ui_window_init(&window, 64, 64, FFLY_UI_WD_HOG, 128, 128);

	ff_u8_t static pixels[64*64*4];
	ffly_mem_set(pixels, 144, 64*64*4);

	ffly_fontp font;
	ffly_ui_textp text;
	text = _ui_text_creat("012");
	font = _font_new();
	_font_init(font, _font_driver_typo);

	_font_face(font, "test.mcd");
	font->driver.scale(0);
	_ui_text_font(text, font);

	_ui_text_draw(text, pixels, 0, 0, 64, 64);

	_font_destroy(font);
	_ui_text_destroy(text);

	_ui_window_write(&window, pixels, 64, 64, 0, 0);
}

void static de_init(void) {
	free(tex0);
	_ui_window_de_init(&window);
	_ui_btn_destroy(fontforge);
}

void static _tick(void) {
	if (!(flags&CTFF)) {
		workshop.de_init();
		ws_fontforge();
	}
}

void ws_front(void) {
	workshop.de_init = de_init;
	tick = _tick;
	init();
}
