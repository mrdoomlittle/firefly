# include "../ffint.h"
# include "../types.h"
# include "../system/nanosleep.h"
# include "../graphics/frame_buff.h"
# include "../graphics.h"
# include "../m.h"
# include "../system/io.h"
# include "../graphics/pipe.h"
# include "../linux/time.h"
# include "../log.h"
# include "../context.h"
# include "../graphics/dk.h"
ff_i8_t(*ffe_tick)(void);

ff_u16_t static sf;
ff_u64_t static cc = 0;
void ffe_loop(void) {
	ff_i8_t rs;
	struct timespec start, stop, now, sstart;
	struct ffly_meminfo mi;
	now.tv_sec = 0;
	sstart.tv_sec = 0;
	clock_gettime(CLOCK_MONOTONIC, &sstart);
_again:
	clock_gettime(CLOCK_MONOTONIC, &now);
	if((now.tv_sec-sstart.tv_sec)>=1) {
		_ff_log("cycles per second: %u\n", cc);
		cc = 0;
		clock_gettime(CLOCK_MONOTONIC, &sstart);
	}

	sf = BRON_CONTEXT->stack;
	bron_start();
	rs = ffe_tick();

//	ffly_nanosleep(0, 5454545454);
	clock_gettime(CLOCK_MONOTONIC, &start);
	ffly_grp_unload(&__ffly_grp__);

	ffly_fb_copy(__ctx(hl_fb));

	bron_finish();
	bron_done();

	ffly_grdk_run();

	BRON_CONTEXT->stack = sf;

	ffly_fb_yank(__ctx(hl_fb));

	clock_gettime(CLOCK_MONOTONIC, &stop);

	ff_log("time taken to prossess graphics{bron}, sec: %u, ns: %u\n", stop.tv_sec-start.tv_sec, stop.tv_nsec-start.tv_nsec);

	ffly_printf("cycle count: %u\n", cc++);
	ffly_meminfo(&mi);
	ffly_dmi(&mi);
	ffly_fdrain(ffly_out);

	if (!rs)
		goto _again;
}
