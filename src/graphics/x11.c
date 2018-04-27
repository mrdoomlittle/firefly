# include "x11.h"
# include "../macros.h"
# include "../system/errno.h"
# include "../system/keyborad.h"
int unsigned static keycodes[] = {
    _ffly_key_w,
    _ffly_key_a,
    _ffly_key_s,
    _ffly_key_d
};

ff_u8_t ffly_x11_convert_btnno(int unsigned __btn) {
	return __btn;
}

ffly_keycode_t ffly_x11_convert_keycode(int unsigned __keycode) {
	switch(__keycode) {
		case 25: return _ffly_key_w;
		case 38: return _ffly_key_a;
		case 39: return _ffly_key_s;
		case 40: return _ffly_key_d;
	}
	return 0xff;
}

ff_err_t ffly_x11_query_pointer(struct ffly_x11_wd *__wd, ff_i16_t *__root_xa, ff_i16_t *__root_ya, ff_i16_t *__wd_xa, ff_i16_t *__wd_ya) {
    Window root, child; // not needed for now
    int root_xa, root_ya, wd_xa, wd_ya;
    int unsigned mask;
    XQueryPointer(__wd->d, XRootWindow(__wd->d, 0), &root, &child, &root_xa, &root_ya, &wd_xa, &wd_ya, &mask);    
    __ffly_inna(__root_xa, root_xa);
    __ffly_inna(__root_ya, root_ya);
    __ffly_inna(__wd_xa, wd_xa);
    __ffly_inna(__wd_ya, wd_ya);
    return FFLY_SUCCESS;
}
