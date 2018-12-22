# ifndef __ffly__btn__h
# define __ffly__btn__h
# include "../ffint.h"
# include "../types.h"
# include "../dep/bzero.h"
# include "../ffly_def.h"
# include "../event.h"
# include "../pallet.h"
#define FFLY_UI_BT_HOG 0x08
#define FFLY_UI_BT_ENABLED 0x02
#define FFLY_UI_BT_TEXT 0x04

#define _ui_btn_creat(...)\
	ffly_gui_btn_creat(__VA_ARGS__)
#define _ui_btn_sched(...)\
	ffly_gui_btn_sched(__VA_ARGS__)
#define _ui_btn_enable(...)\
	ffly_gui_btn_enable(__VA_ARGS__)
#define _ui_btn_disable(...)\
	ffly_gui_btn_disable(__VA_ARGS__)
#define _ui_btn_destroy(...)\
	ffly_gui_btn_destroy(__VA_ARGS__)
struct ffly_gui_btn {
	ff_u8_t id;
	struct ffly_pallet texture;
	ff_i8_t pressed;
	ff_i8_t hovering;
    ff_u16_t x, y;
    ff_u16_t width, height;

	void(*get)(ff_u8_t, long long, void*);
	void *gt_arg;
    ff_i16_t *pt_x, *pt_y;
	ff_i8_t *pt_state;
   	ff_i8_t enabled;
	ff_u8_t flags;
	ff_u32_t sched_id;
	ff_i8_t c;
	ff_u32_t co, ch;
	void *arg_p;
	void(*press)(struct ffly_gui_btn*, void*);
	void(*release)(struct ffly_gui_btn*, void*);
    void(*hover)(struct ffly_gui_btn*, void*);
	struct ffly_gui_btn *prev, *next;
};

typedef struct ffly_gui_btn* ffly_gui_btnp;
# ifdef __cplusplus
extern "C" {
# endif
void ffly_gui_btn_enable(ffly_gui_btnp);
void ffly_gui_btn_disable(ffly_gui_btnp);
ffly_gui_btnp ffly_gui_btn_creat(ff_u8_t*, ff_u16_t, ff_u16_t, ff_u8_t, ff_u16_t, ff_u16_t, void(*)(ff_u8_t, long long, void*), void*);
void ffly_gui_btn_sched(ffly_gui_btnp);
void ffly_gui_btn_destroy(ffly_gui_btnp);
void ffly_gui_btn_init(ffly_gui_btnp, ff_u8_t*, ff_u16_t, ff_u16_t, ff_u8_t, ff_u16_t, ff_u16_t, void(*)(ff_u8_t, long long, void*), void*);
ff_err_t ffly_gui_btn_draw(ffly_gui_btnp);
ff_err_t ffly_gui_btn_enable_ir(ffly_gui_btnp);
ff_i8_t ffly_gui_btn_handle(void*);
ff_i8_t ffly_gui_btn_hovering(ffly_gui_btnp);
ff_i8_t ffly_gui_btn_pressed(ffly_gui_btnp);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace gui {
class btn {
    public:
    void clear() {
        ffly_bzero(&this->raw, sizeof(struct ffly_gui_btn));
    }
    types::err_t draw(types::pixelmap_t __pixelbuff, u16_t __width, u16_t __height) {
        return ffly_gui_btn_draw(&this->raw, __pixelbuff, __width, __height);
    }
    types::err_t enable_ir() {return ffly_gui_btn_enable_ir(&this->raw);}
    types::bool_t pressed() {return ffly_gui_btn_pressed(&this->raw);}
    types::bool_t hovering() {return ffly_gui_btn_hovering(&this->raw);}
    types::err_t handle(types::event_t& __event) {return ffly_gui_btn_handle(&this->raw, &__event);}
    void set_width(u16_t __width) {this->raw.width = __width;}
    void set_height(u16_t __height) {this->raw.height = __height;}
    void set_xa(u16_t __xa) {this->raw.xa = __xa;}
    void set_ya(u16_t __ya) {this->raw.ya = __ya;}
    void set_pt_xa(i16_t& __xa_p) {this->raw.pt_xa = &__xa_p;}
    void set_pt_ya(i16_t& __ya_p) {this->raw.pt_ya = &__ya_p;}
    void enable() {this->raw.enabled = ffly_true;}
    void disable() {this->raw.enabled = ffly_false;}
    void set_arg_p(void *__arg_p){this->raw.arg_p = __arg_p;}
    void press_call(void(*__press_call)(ffly_gui_btnp, void*)){this->raw.press_call = __press_call;}
    void hover_call(void(*__hover_call)(ffly_gui_btnp, void*)){this->raw.hover_call = __hover_call;}
    void set_texture(types::pixelmap_t __texture) {this->raw.texture = __texture;}
    struct ffly_gui_btn raw;
};
}
}
}
# endif
# endif /*__ffly__btn__h*/
