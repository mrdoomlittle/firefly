# include "io.h"
# include <stdarg.h>
# include <mdlint.h>
# include "mutex.h"
FILE *ffly_out = NULL;
FILE *ffly_log = NULL;
FILE *ffly_err = NULL;
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
void static ffly_print(FILE *__stream, char const *__s, va_list __args) {
	if (__stream == NULL || __s == NULL) return;
	ffly_mutex_lock(&mutex);
	fprintf(__stream, "firefly; ");
	fflush(__stream);

	vfprintf(__stream, __s, __args);
	fflush(__stream);
	ffly_mutex_unlock(&mutex);
}

void ffly_fprintf(FILE *__stream, char const *__s, ...) {
	va_list args;
	va_start(args, __s);
	ffly_print(__stream, __s, args);
	va_end(args);
}

void ffly_printf(char const *__s, ...) {
	va_list args;
	va_start(args, __s);
	ffly_print(ffly_out, __s, args);
	va_end(args);
}

