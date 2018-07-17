# include "../bh.h"
# include "../stdio.h"
# include "../brick.h"
//static struct ff_bh bh;
ff_err_t ffmain(int __argc, char const *__argv[]) {
	ff_bh_open(&bh);
	ff_bh_connect(&bh, "127.0.0.1", 21299);
	
	ff_err_t err;
	ff_u32_t b;
	b = ff_bh_bnew(&bh, _ff_brick_256, &err);

	ff_bh_disconnect(&bh);
	ff_bh_close(&bh);
}
