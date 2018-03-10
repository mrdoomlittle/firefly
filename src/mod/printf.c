# include "printf.h"
# include "../call.h"
# include "../mod.h"
# include "malloc.h"
# include "../ffly_def.h"
void static out(char const*, va_list);
ffpap static gen(char*, char const*, va_list);

void ffly_printf(char const *__format, ...) {
	return;
	va_list args;
	va_start(args, __format);

	out(__format, args);	

	va_end(args);
}

void ffly_fprintf(void *__file, char const *__format, ...) {

}

void
out(char const *__format, va_list __args) {
	ffpap head;
	char buf[1024];
	if (!(head = gen(buf, __format, __args))) {
		// error
	}

	ffmod_ring(_ffcal_printf, NULL, &head);
	ffpap cur = head, bk;
	while(cur != NULL) {
		bk = cur;
		cur = cur->next;
		free(bk);
	}
}


ffpap
gen(char *__buf, char const *__format, va_list __args) {
	char const *p = __format;
	char *bufp = __buf;

	ffpap head = NULL, cur = NULL, bk;
	while(*p != '\0') {
		bk = cur;
		if (!(cur = (ffpap)malloc(sizeof(struct ffpa)))) {
			// error
		}

		if (bk != NULL)
			bk->next = cur;

		if (!head)
			head = cur;

		if (*p == '%') {
			p++;	
		} else {
			cur->p = p;	
			while(*p != '%' && *p != '\0') p++;
			cur->end = p;
			cur->type = _ffpa_seg;
		}
	}

	cur->next = NULL;
	return head;
}
