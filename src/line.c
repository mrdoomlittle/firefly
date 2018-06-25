# include "line.h"
# include "system/printf.h"
static char line[1024];
static char *cur = line;

# define cursorat \
	(line+((ff_uint_t)(((ff_int_t)(cur-line))+l_cursor_pos)))

ff_int_t l_cursor_pos = 0;
char static temp;
void ffly_l_show(FF_FILE *__file) {
	temp = *cursorat;
	*cursorat = '#';
	*cur = ' ';
	ffly_fprintf(__file, "\e[2K\r");
	*(cur+1) = '\0';

	ffly_fprintf(__file, "~: %s", line);
	ffly_fprintf(__file, "\e[%uD", ((cur-line)-(ff_uint_t)(((ff_int_t)(cur-line))+l_cursor_pos))+1);
	ffly_fdrain(__file);
	*cursorat = temp;
}

void ffly_l_put(char __c) {
	char *p = cursorat;
	char *e = cur;
	while(e != p) {
		*e = *(e-1);
		e--;
	}

	*p = __c;
	cur++;
}

void ffly_l_del(void) {
	if (!(((ff_int_t)(cur-line))+l_cursor_pos))
		return;
	char *p = cursorat-1;
	char *e = cur;
	while(p != e) {
		*p = *(p+1);
		p++;
	}
	cur--;
}
# include "dep/mem_cpy.h"
ff_uint_t
ffly_l_load(char *__dst) {
	ff_uint_t l = cur-line;
	ffly_mem_cpy(__dst, line, l);
	return l;
}

void ffly_l_reset(void) {
	cur = line;
	l_cursor_pos = 0;
}
