# include "alloca.h"
# include "mem_alloc.h"
# include "mem_free.h"
# include "../system/err.h"
# define ALLOCA_SSIZE 400
static mdl_u8_t *p = NULL;
static mdl_u8_t *fresh = NULL;
void* ffly_alloca(mdl_uint_t __size, ffly_err_t *__err) {
    if (!p) {
        p = (mdl_u8_t*)__ffly_mem_alloc(ALLOCA_SSIZE);
		fresh = p;
	}

	if (fresh>(p+ALLOCA_SSIZE)) {
		ffly_errmsg("alloca failure.\n");
		return NULL;
	}
    void *ret = (void*)fresh;
	fresh+=__size;
	return ret;
}

void ffly_trim(mdl_uint_t __n) {
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
