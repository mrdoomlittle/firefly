# include <stdio.h>
# include <stdarg.h>
/*
# define POINTER_COORDS 0
mdl::u8_t static ffly_arg_id(char *__arg) {
	if (!strcmp("", __arg)) {

	}
}
*/
void ffly_printf(FILE *__stream, char *__str, ...) {
	va_list args;
	va_start(args, __str);
/*
	char *itr = __str;
	while(itr != '\0') {
		if (*itr == '#' && *(itr++) == '#')
			
	}
*/
	printf("firefly; ");
	vfprintf(__stream, __str, args);
	va_end(args);
}
