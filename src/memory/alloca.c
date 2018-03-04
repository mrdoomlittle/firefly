# include "alloca.h"
# include "mem_alloc.h"
# include "mem_free.h"
# include "../system/err.h"
# include "../mode.h"
# include "../system/io.h"
# include "../system/config.h"
# include "../dep/mem_cpy.h"
static mdl_u8_t *p = NULL;
static mdl_u8_t *fresh = NULL;
static mdl_u8_t *end;

void static **amend[20];
void static ***bot = amend;

void ffly_alrr() {
	mdl_u8_t *tmp = p;
	mdl_u8_t *end = fresh;
	ffly_alrig();
	ffly_alsld(tmp, end-tmp);
	fresh+=end-tmp;
	void ***cur = amend;
	while(cur != bot) {
		ffly_printf("......\n");
		**cur = (void*)(p+(((mdl_u8_t*)**cur)-tmp));
		cur++;
	}
	bot = amend;
}

void ffly_alad(void **__p) {
	*(bot++) = __p;
}

mdl_uint_t ffly_alssz() {
	return fresh-p;
}

void ffly_alsst(void *__p) {
	ffly_mem_cpy(__p, p, fresh-p);
}

void ffly_alsld(void *__p, mdl_uint_t __size) {
	ffly_mem_cpy(p, __p, __size);
}

void ffly_alrig() {
	p = (mdl_u8_t*)__ffly_mem_alloc(__ffly_sysconf__.alssize);
	fresh = p;
	end = p+__ffly_sysconf__.alssize;
}

void ffly_alss(void *__p, mdl_uint_t __size) {
	p = (mdl_u8_t*)__p;
	fresh = p;
	end = p+__size;
}

void* ffly_alloca(mdl_uint_t __size, ffly_err_t *__err) {
	if (fresh>=end) {
		ffly_errmsg("alloca failure.\n");
		return NULL;
	}
    void *ret = (void*)fresh;
	fresh+=__size;
	__ffmod_debug {
		if (fresh>=end)
			ffly_printf("warning: alloca, memory extends over end of stack, end extends: %u bytes.\n", fresh-end);
		ffly_printf("alloca, size: %u, off: %u\n", __size, fresh-p);	
	}
	return ret;
}

void ffly_trim(mdl_uint_t __n) {
	if (fresh-__n < p) {
		ffly_printf("warning: someone has tryed to trim to much.\n");
		fresh = p; // reset just in case
		return;
	}
	fresh-=__n;
}

void *ffly_frame() {
	return fresh;
}

void ffly_collapse(void *__frame) {
	fresh = __frame;
}

void ffly_alloca_cleanup() {
	if (p != NULL)
		__ffly_mem_free(p);
}
