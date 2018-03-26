# include "printf.h"
# include "../call.h"
# include "ring.h"
# include "malloc.h"
# include "../ffly_def.h"
# include "copy.h"
void static out(char const*, va_list);
ffpap static gen(char*, char const*, va_list);

void ffly_printf(char const *__format, ...) {

}

void printf(char const *__format, ...) {
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
	
	setmalopt(FF_MAL_O_OSD);
	ffpap cur = head, bk = NULL, dup, rr, top = NULL;
	while(cur != NULL) {
		dup = (ffpap)malloc(sizeof(struct ffpa));
		if (bk != NULL) {
			bk->next = dup;
			scopy(rr, bk, sizeof(struct ffpa));
			setmalopt(FF_MAL_O_LOC);
			free(bk);
			setmalopt(FF_MAL_O_OSD);
		}

		mdl_uint_t l = cur->end-cur->p;
		void *pre = cur->p;
		cur->p = (ffpap)malloc(l);
		cur->end = cur->p+l;
		scopy(cur->p, pre, l);

		if (!top)
			top = dup;

		bk = cur;
		cur = cur->next;		
		rr = dup;
	}

	if (bk != NULL) {
		bk->next = NULL;
		scopy(rr, bk, sizeof(struct ffpa));
		setmalopt(FF_MAL_O_LOC);
		free(bk);
	}

	ffmod_ring(_ffcal_printf, NULL, &top);
	setmalopt(FF_MAL_O_LOC);
}


ffpap
gen(char *__buf, char const *__format, va_list __args) {
	setmalopt(FF_MAL_O_LOC);
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
			if (*p == 'u') {
				*(mdl_u32_t*)bufp = va_arg(__args, mdl_u32_t);
				cur->p = bufp;
				bufp+=sizeof(mdl_u32_t);
				cur->end = bufp;
				cur->type = _ffpa_u;
			}
			p++;
		} else {
			cur->p = bufp;	
			while(*p != '%' && *p != '\0')
				*(bufp++) = *(p++);
			cur->end = bufp;
			cur->type = _ffpa_seg;
		}
	}

	cur->next = NULL;
	return head;
}
