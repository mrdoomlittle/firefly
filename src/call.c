# include "call.h"
# include "system/string.h"
# include "system/io.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"
void static
ffcall_printf(void *__ret, void *__params) {
	ffpap arg = *(ffpap*)__params;
	char buf[1024];
	char *bufp = buf;
	while(arg != NULL) {
		if (arg->type == _ffpa_seg) {
			ff_u8_t *p = (ff_u8_t*)arg->p;
			while(p != arg->end)
				*(bufp++) = *(p++);
		} else if (arg->type == _ffpa_u)
			bufp+= ffly_nots(*(ff_u32_t*)arg->p, bufp);
		arg = arg->next;
	}
	*bufp = '\0';

	ffly_printf("%s", buf);
}

void static
ffcall_fprintf(void *__ret, void *__params) {

}

void static
ffcall_malloc(void *__ret, void *__params) {
	*(void**)__ret = __ffly_mem_alloc(*(ff_uint_t*)__params);
}

void static
ffcall_free(void *__ret, void *__params) {
	__ffly_mem_free(*(void**)__params);
}

void static
ffcall_realloc(void *__ret, void *__params) {
	*(void**)__ret = __ffly_mem_realloc(*(void**)__params, *(ff_uint_t*)((ff_u8_t*)__params+8));
}

static void(*call[])(void*, void*) = {
	ffcall_printf,
	ffcall_fprintf,
	ffcall_malloc,
	ffcall_free,
	ffcall_realloc
};

void ffcall(ff_u8_t __no, void *__ret, void *__params) {
	call[__no](__ret, __params);
}



