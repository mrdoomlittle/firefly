# include "../bh.h"
# include "../storage/reservoir.h"
# include "../brick.h"
# include "../system/sched.h"
# include "../system/thread.h"
# include "../system/io.h"
# include "../dep/mem_cmp.h"
ff_i8_t static done = -1;
void static*
start(void *__arg) {
	ff_bhs_start();
	done = 0;
}

ff_err_t ffmain(int __argc, char const *__argv[]) {
	ffly_scheduler_init(0);
	ffly_reservoir_init(&__ffly_reservoir__, 0, "temp.resv");
	ff_bhs_prep();
	ff_bhs_open();

	ff_tid_t t;
	ffly_thread_create(&t, start, NULL);

	char buf[128];
	while(1) {
 		ffly_printf("~: ");
		ffly_fdrain(ffly_out);
		ffly_rdline(buf, 200, ffly_in);
		if (!ffly_mem_cmp(buf, "end", 3))
			break;
	}

	ffly_printf("waiting for ctl-c\n");
	ffly_fdrain(ffly_out);
	while(done == -1);
	ff_bhs_closedown();
 	ff_bhs_cleanup();
	ffly_brick_cleanup();
	ffly_reservoir_de_init(&__ffly_reservoir__);
	ffly_scheduler_de_init();
}
