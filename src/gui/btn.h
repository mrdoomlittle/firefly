# ifndef __ffly__btn__h
# define __ffly__btn__h
# include <mdlint.h>
# include "../types/pixelmap_t.h"
# include "../types/bool_t.h"
# include "../types/err_t.h"
# include "../types/event_t.h"
# include "../data/bzero.h"
# include "../ffly_def.h"
struct ffly_ui_btn {
	ffly_pixelmap_t texture;
	ffly_bool_t pressed;
	ffly_bool_t hovering;
    mdl_u16_t xa, ya;
    mdl_u16_t width, height;
    mdl_i16_t *pt_xa, *pt_ya;
    ffly_bool_t enabled;
// for later
    void *arg_p;
    void(*press_call)(void*);
};
# ifdef __cplusplus
extern "C" {
# endif
ffly_err_t ffly_ui_btn_enable_ir(struct ffly_ui_btn*);
ffly_err_t ffly_ui_btn_handle(struct ffly_ui_btn*, ffly_event_t*);
ffly_bool_t ffly_ui_btn_hovering(struct ffly_ui_btn*);
ffly_bool_t ffly_ui_btn_pressed(struct ffly_ui_btn*);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace gui {
class btn {
    public:
    void clear() {
        ffly_bzero(&this->raw, sizeof(struct ffly_ui_btn));
    }
    types::err_t enable_ir() {return ffly_ui_btn_enable_ir(&this->raw);}
    types::bool_t pressed() {return ffly_ui_btn_pressed(&this->raw);}
    types::bool_t hovering() {return ffly_ui_btn_hovering(&this->raw);}
    types::err_t handle(types::event_t& __event) {return ffly_ui_btn_handle(&this->raw, &__event);}
    void set_width(u16_t __width) {this->raw.width = __width;}
    void set_height(u16_t __height) {this->raw.height = __height;}
    void set_xa(u16_t __xa) {this->raw.xa = __xa;}
    void set_ya(u16_t __ya) {this->raw.ya = __ya;}
    void set_pt_xa(i16_t& __xa_p) {this->raw.pt_xa = &__xa_p;}
    void set_pt_ya(i16_t& __ya_p) {this->raw.pt_ya = &__ya_p;}
    void enable() {this->raw.enabled = ffly_true;}
    void disable() {this->raw.enabled = ffly_false;}
    void set_texture(types::pixelmap_t __texture) {this->raw.texture = __texture;}
    struct ffly_ui_btn raw;
};
}
}
}
# endif
# endif /*__ffly__btn__h*/
