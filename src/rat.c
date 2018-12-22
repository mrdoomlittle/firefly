# include "rat.h"
# include <stdarg.h>
# include "system/io.h"
# include "system/file.h"
/*
	like log but outputs everything - only used for allocr at the time 
*/
ff_u8_t static level = _ff_rat_2;

void ff_rat_put(ff_u8_t __level) {
	level = __level;
}

void ff_rat(ff_u8_t __level, void(*__out)(char const*, va_list), char const *__format, ...) {
	if (__level != level) return;
	va_list args;
	va_start(args, __format);
	__out(__format, args);
	va_end(args);
}
