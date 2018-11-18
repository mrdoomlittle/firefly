# include "../bh.h"
# include "../stdio.h"
# include "../string.h"
# include "../brick.h"
# include "../system/util/ff5.h"
//static struct ff_bh bh;
ff_err_t ffmain(int __argc, char const *__argv[]) {
	ff_bh_open(&bh);
	ff_bh_connect(&bh, "192.168.0.111", 40960);	
	ff_err_t err;
/*	
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
*/
//	ff_bh_bnewm(&bh, _ff_brick_256, b, n);
//	char buf[1024];

//	*(buf+ffly_ff5_enc(b, buf, n*sizeof(ff_u32_t))) = '\0';
//	printf("brick key: %s\n", buf);
//	ff_bh_bridm(&bh, b, n);

	char buf0[256];
	ff_u32_t b;
	if (*__argv[1] == '#') {
		ffly_ff5_dec(__argv[2], &b, strlen(__argv[2]));
		if (!ff_bh_bexist(&bh, b, &err)) {
			ff_bh_bopen(&bh, b);
			ff_bh_bread(&bh, b, buf0, 256, 0);
			ff_bh_bclose(&bh, b);
			printf("%s\n", buf0);
		} else
			printf("brick/s do not exist.\n");
	} else {
		strcpy(buf0, __argv[2]);
		b = ff_bh_bnew(&bh, _ff_brick_256, &err);
		char buf[1024];
		*(buf+ffly_ff5_enc(&b, buf, sizeof(ff_u32_t))) = '\0';
		printf("brick key: %s\n", buf);
		ff_bh_bopen(&bh, b);
		ff_bh_bwrite(&bh, b, buf0, 100, 0);
		ff_bh_bclose(&bh, b);
	}

	ff_bh_disconnect(&bh);
	ff_bh_close(&bh);
}
