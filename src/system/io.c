# include <stdio.h>
# include <stdarg.h>
# include <mdlint.h>
# include "mutex.h"

ffly_mutex_t static mutex = FFLY_MUTEX_INIT;
void ffly_printf(FILE *__stream, char const *__str, ...) {
	ffly_mutex_lock(&mutex);
	va_list args;
	va_start(args, __str);

	fprintf(__stream, "firefly; ");
	vfprintf(__stream, __str, args);
	va_end(args);
	ffly_mutex_unlock(&mutex);
}
