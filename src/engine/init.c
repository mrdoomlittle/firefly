# include "init.h"
# include "../graphics.h"
# include "view.h"
# include "../memory/plate.h"
# include "../system/sched.h"
# include "../graphics/pipe.h"
# include "../graphics/frame_buff.h"
# include "../context.h"
ff_u16_t ffe_iflgs = 0xffff;
#define hasbit(__what)\
	!(ffe_iflgs&__what)

ff_i8_t _ffe_init(void) {
	__ctx(vpw) = EF_WIDTH;
	__ctx(vph) = EF_HEIGHT;
	ff_err_t err;
	ffly_scheduler_init(0);
	if (hasbit(FF_E_GR)) {
		ffly_bron_driver(_bron_dd_nought, &BRON_CONTEXT->driver);
		BRON_CONTEXT->stack = 0;
		bron_setctx(bron_ctx_new());
		__ctx(ll_fb) = bron_fb_new(EF_WIDTH, EF_HEIGHT);
		__ctx(rb) = bron_rb_new(EF_WIDTH, EF_HEIGHT);
		bron_fb_set(__ctx(ll_fb));
		bron_rb_bind(__ctx(rb), __ctx(ll_fb));
		struct bron_viewport vp = {
			0, 0, EF_WIDTH, EF_HEIGHT
		};
		bron_viewport(&vp);
		bron_lookat(EF_WIDTH/2, EF_HEIGHT/2, 0, 1, 1, 1);

		bron_done();
		ffly_grp_prepare(&__ffly_grp__, 200);

		plfb(fb_creat(EF_WIDTH, EF_HEIGHT, 4, &err));
	}
}

ff_i8_t _ffe_de_init(void) {
	if (hasbit(FF_E_GR)) {
		bron_rb_destroy(__ctx(rb));
		bron_fb_destroy(__ctx(ll_fb));
		bron_done();
		
		ffly_grp_cleanup(&__ffly_grp__);
		ffly_grj_cleanup();
		ffly_frame_buff_del(__ctx(hl_fb));
	}

	ffly_tile_cleanup();
	ffly_scheduler_de_init();
	ffly_plate_cleanup();
}
