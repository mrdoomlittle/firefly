# include "types/err_t.h"
# include "dep/mem_cpy.h"
# include "system/thread.h"
# include "stdio.h"
# include "malloc.h"
void* thr(void *__arg_p) {
	printf("Hello...\n");
}

ffly_err_t ffmain(int __argc, char const *__argv[]) {
	printf(".....\n");
	ffly_tid_t t0;
	ffly_thread_create(&t0, thr, NULL);

}
