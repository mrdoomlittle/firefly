# include <stdio.h>
# include <stdarg.h>
# include <mdlint.h>
# include "mutex.h"
/*
# define POINTER_COORDS 0
mdl::u8_t static ffly_arg_id(char *__arg) {
	if (!strcmp("", __arg)) {

	}
}
*/
void ffly_open(void *__p, mdl_u8_t __kind) {


}

ffly_mutex_t static mutex = 0;
void ffly_printf(FILE *__stream, char const *__str, ...) {
	ffly_mutex_lock(&mutex);
	va_list args;
	va_start(args, __str);

	fprintf(__stream, "firefly; ");
	vfprintf(__stream, __str, args);
	va_end(args);
	ffly_mutex_unlock(&mutex);
}
