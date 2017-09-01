# include <stdio.h>
# include <stdarg.h>
# include <mdlint.h>
/*
# define POINTER_COORDS 0
mdl::u8_t static ffly_arg_id(char *__arg) {
	if (!strcmp("", __arg)) {

	}
}
*/
void ffly_open(void *__p, mdl_u8_t __kind) {


}

void ffly_printf(FILE *__stream, char const *__str, ...) {
	va_list args;
	va_start(args, __str);

	printf("firefly; ");
	vfprintf(__stream, __str, args);
	va_end(args);
}
