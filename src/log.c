# include "system/io.h"
# include <stdarg.h>
void _ff_log(char const *__format, ...) {
	va_list args;
	va_start(args, __format);
	ffly_vfprintf(ffly_log, __format, args);
	va_end(args);
}
