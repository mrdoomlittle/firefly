# include "../bh.h"
# include "../stdio.h"
# include "../brick.h"
//static struct ff_bh bh;
ff_err_t ffmain(int __argc, char const *__argv[]) {
	ff_bh_open(&bh);
	ff_bh_connect(&bh, "127.0.0.1", 21299);
	
	ff_err_t err;
	
	ff_uint_t const n = 8;
	ff_u32_t b[n];

	ff_uint_t i;

	i = 0;
	while(i != n) {
		b[i++] = ff_bh_bnew(&bh, _ff_brick_256, &err);
	}

	i = 0;
	while(i != n) {
		ff_bh_brid(&bh, b[i++]);
	}


	ff_bh_disconnect(&bh);
	ff_bh_close(&bh);
}
