# include "types/err_t.h"
# include "memory/allocr.h"
# include "system/io.h"
# include "system/thread.h"
# include "system/config.h"
ffly_err_t ffmain(int, char const**);
int main(int __argc, char const **__argv) {
    ffly_ar_init();
    ffly_io_init();
/*
	if (__argc < 2) {
		ffly_printf("please provide sysconf.\n");
		goto _end;
	}
	ffly_ld_sysconf(*(__argv+1));
*/
	ffmain(__argc, __argv);
	_end:
//	ffly_free_sysconf();
//	ffly_thread_cleanup();
    ffly_io_closeup();
    ffly_ar_cleanup();
    return 0; 
}
