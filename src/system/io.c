# include "io.h"
# include <stdarg.h>
# include <mdlint.h>
# include "mutex.h"
# define ffly_in stdin
FILE *ffly_out;
FILE *ffly_log;
FILE *ffly_err;
ffly_err_t ffly_io_init() {
	ffly_out = fopen("/dev/tty", "w");
	ffly_log = fopen("log", "w");
	ffly_err = fopen("err", "w");
}

void ffly_io_closeup() {
	fflush(ffly_out);
	fflush(ffly_log);
	fflush(ffly_err);

	fclose(ffly_out);
	fclose(ffly_log);
	fclose(ffly_err);
}

ffly_mutex_t static mutex = FFLY_MUTEX_INIT;
void ffly_printf(FILE *__stream, char const *__s, ...) {
	ffly_mutex_lock(&mutex);
	va_list args;
	va_start(args, __s);

	fprintf(__stream, "firefly; ");
	vfprintf(__stream, __s, args);
	va_end(args);
	fflush(stdout);
	ffly_mutex_unlock(&mutex);
}
