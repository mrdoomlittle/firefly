# include "err.h"
# include "io.h"
ff_err_t ffly_errval;
void _ffly_errmsg(char const *__file, int unsigned __line, char const *__s, ...) {
	char buf[1024];
	*buf = '\0';
	ff_uint_t l;
	va_list args;
	va_start(args, __s);
	l = ffly_vsprintf(buf, __s, args);
	va_end(args);
	ffly_fprintf(ffly_out, "%s:%u, %s", __file, __line, buf);
}
