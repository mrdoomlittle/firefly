# include "font_forge.h"
# include "../ffint.h"
# include "../malloc.h"
# include "../ui/btn.h"
# include "../system/io.h"
# include "front.h"
static ffly_ui_btnp back;
struct dent {
	ff_u8_t r, g, b, a;
};

#define DENT_SHIFT 2
#define DENT_SIZE (1<<DENT_SHIFT)
static struct dent *map;
static ff_uint_t nd;

enum {
	_bt_front
};

#define GOBACK 0x01
void static clear(void) {
	struct dent *d, *e;
	d = map;
	e = d+nd;
	while(d != e) {
		d->r = 0;
		d->g = 0;
		d->b = 0;
		d->a = 255;
		d++;
	}
}

#define dentat(__x, __y) \
	(map+(__x)+((__y)*(ws_width>>DENT_SHIFT)))

ff_u8_t static flags = 0xff;
void static
bt_press(ffly_ui_btnp __btn, long long __arg) {
	ffly_printf("button press.\n");
	if (__btn->id == _bt_front) {
		ffly_printf("back to front.\n");
		flags ^= GOBACK;
	}
}

void static
bt_release(ffly_ui_btnp __btn, long long __arg) {
	ffly_printf("button, release.\n");
}

void static bt_hover(ffly_ui_btnp __btn, long long __arg) {
	ffly_printf("button hover.\n");
}

ff_u8_t static *tex0;
static void de_init(void) {
	free(tex0);
	free(map);
	_ui_btn_destroy(back);
}

void static _tick(void);

# include "../graphics/format.h"
# include "../graphics/pixel.h"
void ws_fontforge(void) {
	ffly_fprintf(ffly_log, "fontforge.\n");
	workshop.de_init = de_init;
	tick = _tick;
	map = (struct dent*)malloc((nd = ((ws_width>>DENT_SHIFT)*(ws_height>>DENT_SHIFT)))*sizeof(struct dent));
	tex0 = (ff_u8_t*)malloc(144*23*4);
	ff_u8_t col[4] = {0, 0, 0, 100};
	ffly_pix_set(col, tex0, 144*23, _FF_FORMAT_RGBA);
	ffly_ui_btnp btn;
	btn = _ui_btn_creat(FFLY_UI_BT_HOG, workshop.get, 0);
	btn->press = bt_press;
	btn->hover = bt_hover;
	btn->release = bt_release;
	btn->id = _bt_front;
	back = btn;
	struct ffly_ui_btn_struc btn_set = {
		.width = 144,
		.height = 23,
		.x = 0,
		.y = 0,
		.text = {
			"back button",
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
}

void _tick(void) {
	if (!(flags&GOBACK)) {
		de_init();
		ws_front();
		flags = 0xff;
		return;
	}

	struct dent *d;
	ff_uint_t x, y;
	y = 0;
	while(y != (HEIGHT>>DENT_SHIFT)) {
		x = 0;
		while(x != (WIDTH>>DENT_SHIFT)) {
			d = dentat(x, y);
			ff_uint_t dx, dy;
			dx = x*DENT_SIZE;
			dy = y*DENT_SIZE;

			x++;
		}
		y++;
	}
}
