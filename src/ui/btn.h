# ifndef __ffly__ui__btn__h
# define __ffly__ui__btn__h
# include "../ffint.h"
# include "../types.h"
# include "../dep/bzero.h"
# include "../ffly_def.h"
# include "../event.h"
# include "../pallet.h"
# include "../maths/vec2.h"
# include "text.h"
#define FFLY_UI_BT_ENABLED	0x02
#define FFLY_UI_BT_TEXT		0x04
#define FFLY_UI_BT_HOG		0x08
#define FFLY_UI_BT_PT		0x10

#define FFLY_UI_BT_TEXT_VRT 0x01//vert centre
#define FFLY_UI_BT_TEXT_HZT 0x02//hoz centre
#define FFLY_UI_BT_TEXT_CENTRE (FFLY_UI_BT_TEXT_VRT|FFLY_UI_BT_TEXT_HZT) // centre
#define FFLY_UI_BT_TEXT_LEFT	(0<<2) //left
#define FFLY_UI_BT_TEXT_RIGHT	(1<<2) //right
#define FFLY_UI_BT_TEXT_TOP		(0<<2) //top
#define FFLY_UI_BT_TEXT_BOTTOM 	(1<<2) //bottom

#define FFLY_UI_BT_TA	0x01
/*

*/

#define _ui_btn_creat(...)\
	ffly_ui_btn_creat(__VA_ARGS__)
#define _ui_btn_sched(...)\
	ffly_ui_btn_sched(__VA_ARGS__)
#define _ui_btn_enable(...)\
	ffly_ui_btn_enable(__VA_ARGS__)
#define _ui_btn_disable(...)\
	ffly_ui_btn_disable(__VA_ARGS__)
#define _ui_btn_destroy(...)\
	ffly_ui_btn_destroy(__VA_ARGS__)
#define _ui_btn_fnode_v(__bt, __vec)\
	ffly_ui_btn_fnode(__bt, __vec.x, __vec.y)	
#define _ui_btn_compose(...)\
	ffly_ui_btn_compose(__VA_ARGS__)
#define _ui_btn_set(...)\
	ffly_ui_btn_set(__VA_ARGS__)
#define FF_UI_BS_AREA	0x01
#define FF_UI_BS_POS	0x02
#define FF_UI_BS_TEXT	0x04
/*
	TODO:
		thru flag GLOBAL OR LOCAL 'get func'
		and provide 'get' func as such in context.h struct
*/
#ifdef __uibtn_internal
struct f_node {
	ff_i16_t x, y;
	struct f_node *next;
};
#endif

struct ffly_ui_btn_struc {
	ff_u16_t x, y;
	ff_u16_t width, height;
	struct {
		char const *ptr;
		ff_uint_t len;
		ff_u8_t align;
		ff_u8_t flags;
	} text;
};

struct ffly_ui_btn {
	ff_u8_t id;
	struct ffly_pallet texture;
	ff_i8_t pressed;
	ff_i8_t hovering;
    ff_u16_t x, y;

	/*
		not the button
	*/
    ff_u16_t width, height;

	char const *text;

	struct {
		ff_u16_t xoff, yoff;
		ff_uint_t len;
		ff_u8_t align;
		ff_u8_t flags;
	} textinfo;
/*
	if we want local buttons that are within ui window for example
*/
	void(*get)(ff_u8_t, long long, long long);
	long long gt_arg;

	ff_u8_t *m_bits;

    ff_i16_t *pt_x, *pt_y;
	ff_i8_t *pt_state;

   	ff_i8_t enabled;
	ff_u8_t flags;
	ff_u8_t bits;
	ff_u32_t sched_id;
	ff_i8_t c;
	ff_u32_t co, ch;
	long long arg;
	/*
		i know i could use i func but that leaves the user end getting bogged down
		with switch and or if statments, so for primary things like this it should be a direct call.
		as thay are to be called repeatedly when a state has been met.
	*/
	void(*press)(struct ffly_ui_btn*, long long);
	void(*release)(struct ffly_ui_btn*, long long);
    void(*hover)(struct ffly_ui_btn*, long long);

/*
	inform the user of minor changes
*/
	void(*inform)(ff_u8_t, long long);
	struct ffly_ui_btn *prev, *next;
#ifdef __uibtn_internal
	struct f_node *node;
#else
	void *node;
#endif

};

typedef struct ffly_ui_btn* ffly_ui_btnp;
# ifdef __cplusplus
extern "C" {
# endif
/*
	btn_set to be change into somthing to manipulated/alter the button but also to set and get stuff

	btn_set why? avoid calling too many functions with large address displacments for prep work.
	i think it should be quicker and neater <- nice to look at.

*/
void ffly_ui_btn_usched(ffly_ui_btnp);
void ffly_ui_btn_set(long long, ffly_ui_btnp, ff_u8_t);
void ffly_ui_btn_setarea(ffly_ui_btnp, ff_u16_t, ff_u16_t);
void ffly_ui_btn_setpos(ffly_ui_btnp, ff_u16_t, ff_u16_t);
void ffly_ui_btn_settext(ffly_ui_btnp, char const*);
void ffly_ui_btn_compose(ffly_ui_btnp);
void ffly_ui_btn_fnode(ffly_ui_btnp, ff_i16_t, ff_i16_t);
void ffly_ui_btn_enable(ffly_ui_btnp);
void ffly_ui_btn_disable(ffly_ui_btnp);
ffly_ui_btnp ffly_ui_btn_creat(ff_u8_t, void(*)(ff_u8_t, long long, long long), long long);
void ffly_ui_btn_sched(ffly_ui_btnp);
void ffly_ui_btn_destroy(ffly_ui_btnp);
void ffly_ui_btn_init(ffly_ui_btnp, ff_u8_t, void(*)(ff_u8_t, long long, long long), long long);
ff_err_t ffly_ui_btn_draw(ffly_ui_btnp);
ff_err_t ffly_ui_btn_enable_ir(ffly_ui_btnp);
ff_i8_t ffly_ui_btn_hovering(ffly_ui_btnp);
ff_i8_t ffly_ui_btn_pressed(ffly_ui_btnp);
# ifdef __cplusplus
}
namespace mdl {
namespace firefly {
namespace ui {
class btn {
    public:
    void clear() {
        ffly_bzero(&this->raw, sizeof(struct ffly_ui_btn));
    }
    types::err_t draw(types::pixelmap_t __pixelbuff, u16_t __width, u16_t __height) {
        return ffly_ui_btn_draw(&this->raw, __pixelbuff, __width, __height);
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
    void set_arg_p(void *__arg_p){this->raw.arg_p = __arg_p;}
    void press_call(void(*__press_call)(ffly_ui_btnp, void*)){this->raw.press_call = __press_call;}
    void hover_call(void(*__hover_call)(ffly_ui_btnp, void*)){this->raw.hover_call = __hover_call;}
    void set_texture(types::pixelmap_t __texture) {this->raw.texture = __texture;}
    struct ffly_ui_btn raw;
};
}
}
}
# endif
# endif /*__ffly__ui__btn__h*/
