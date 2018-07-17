# include "chrdump.h"
# include "system/io.h"
# include "system/string.h"
# define ROW_SHIFT 5
# define ROW_LEN (1<<ROW_SHIFT)

void ffly_chrdump(ff_u8_t *__p, ff_uint_t __size) {
	char buf[ROW_LEN+1];
	char *bufp;
	ff_uint_t nw;

	char c;
	nw = __size>>ROW_SHIFT;
	ff_u8_t *p = __p;
	ff_u8_t *e = p+(nw*ROW_LEN);
	while(p != e) {
		bufp = buf;

		ff_u8_t i = 0;
		while(i != ROW_LEN) {
			c = p[i];
			if (!(c>='a' && c <='z')) c = '.';
			*(bufp++) = c;
			i++;
		}

		*bufp = '\0';
		p+=ROW_LEN;
		ffly_printf("%s\n", buf);
	}

	if (p != (e = __p+__size)) {
		bufp = buf;
		while(p != e) {
			c = *p;
			if (!(c>='a' && c <='z')) c = '.';
			*(bufp++) = c;
			p++;
		}
		*bufp = '\0';

		ffly_printf("%s\n", buf);
	}
}
