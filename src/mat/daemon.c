# include "../mtp.h"
# include "../system/err.h"
ff_err_t ffmain(int __argc, char const *__argv[]) {
	ffly_mtpd_prepare("/home/daniel-robson/mount/firefly/matdoc");
	if (_err(ffly_mtpd_open())) {
		goto _end;
	}
	
	ffly_mtpd_start();

	_end:
	ffly_mtpd_close();
	ffly_mtpd_cleanup();
}
