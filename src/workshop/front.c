# include "front.h"
# include "font_forge.h"
# include "../workshop.h"
# include "../system/io.h"
# include "../ui/text.h"
# include "../font.h"
# include "../malloc.h"
# include "../ui/btn.h"
# include "../ui/window.h"
# include "../dep/mem_set.h"
# include "../bitfont.h"
static ffly_ui_btnp fontforge;
static ffly_ui_windowp window;

enum {
	_bt_fontforge
};

ff_u8_t static *tex0, *tex1;
ff_u8_t static flags = 0xff;
#define CTFF 0x01
void static
bt_press(ffly_ui_btnp __btn, long long __arg) {
	ffly_printf("button press.\n");
	if (__btn->id == _bt_fontforge && (flags&CTFF)>0) {
		ffly_printf("fontforge.\n");
		flags ^= CTFF;
	}
}

void static
bt_release(ffly_ui_btnp __btn, long long __arg) {
	ffly_printf("button, release.\n");
}

void static bt_hover(ffly_ui_btnp __btn, long long __arg) {
	ffly_printf("button hover.\n");
}

# include "../graphics/format.h"
# include "../graphics/pixel.h"
void static init(void) {
	tex0 = (ff_u8_t*)malloc(144*23*4);
	ff_u8_t col[4] = {0, 0, 0, 100};
	ffly_pix_set(col, tex0, 144*23, _FF_FORMAT_RGBA);
	ffly_ui_btnp btn;
	btn = _ui_btn_creat(FFLY_UI_BT_HOG, workshop.get, 0);
	btn->press = bt_press;
	btn->hover = bt_hover;
	btn->release = bt_release;
	btn->id = _bt_fontforge;
	fontforge = btn;
	struct ffly_ui_btn_struc btn_set = {
		.width = 144,
		.height = 23,
		.x = 0,
		.y = 0,
		.text = {
			"test button",
			11,
			FFLY_UI_BT_TEXT_CENTRE,
			FFLY_UI_BT_TA
		}
	};

	_ui_btn_set((long long)&btn_set, btn, FF_UI_BS_AREA|FF_UI_BS_POS|FF_UI_BS_TEXT);
	_ui_btn_compose(btn);
	_ui_btn_sched(btn);
	_ui_btn_enable(btn);
	_plt_write(&btn->texture, tex0, 144, 23, 0, 0);

	window = _ui_window_creat(FFLY_UI_WD_HOG);

	struct ffly_ui_window_struc wd_set = {
		.width = 128,
		.height = 128,
		.x = 128,
		.y = 128
	};
	_ui_window_set((long long)&wd_set, window, FF_UI_WS_AREA|FF_UI_WS_POS);
	_ui_window_compose(window);

	ff_u8_t static pixels[128*128*4];
	ffly_mem_set(pixels, 144, 128*128*4);

//	ffly_fontp font;
	ffly_ui_textp text;
	text = _ui_text_creat("0123456789\nabcdefg\nhijklmnop\nqrstuvwxyz");
//	font = _font_new();
//	_font_init(font, _font_driver_typo);

//	_font_face(font, "test.mcd");
//	font->driver.scale(0);
	_ui_text_font(text, 0, _uitext_bitfont);

	_ui_text_draw(text, pixels, 0, 0, 128, 128);

//	_font_destroy(font);
	_ui_text_destroy(text);

	_ui_window_write(window, pixels, 128,128, 0, 0);
}

void static de_init(void) {
	free(tex0);
	_ui_window_destroy(window);
	_ui_btn_destroy(fontforge);
}

# include "../graphics/chamber.h"
# include "../system/nanosleep.h"
void static _tick(void) {
	if (!(flags&CTFF)) {
		workshop.de_init();
		ws_fontforge();
		flags = 0xff;
	}
}

void ws_front(void) {
	workshop.de_init = de_init;
	tick = _tick;
	init();
}
