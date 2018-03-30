# include "pellet.h"
# include "dep/mem_cpy.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "system/io.h"
ffly_pelletp ffly_pellet_mk(mdl_uint_t __size) {
	ffly_pelletp ret = (ffly_pelletp)__ffly_mem_alloc(sizeof(struct ffly_pellet));
	ret->p = (mdl_u8_t*)__ffly_mem_alloc(__size);
	ret->end = ret->p+__size;
	ret->off = 0;
	return ret;
}

void ffly_pellet_put(ffly_pelletp __pel, void const *__p, mdl_uint_t __size) {
	mdl_u8_t *dst = __pel->p+__pel->off;
	if (dst>=__pel->end) {
		ffly_printf("pellet error, put\n");
		return;
	}
	ffly_mem_cpy(dst, __p, __size);
}

void ffly_pellet_get(ffly_pelletp __pel, void *__p, mdl_uint_t __size) {
	mdl_u8_t *src = __pel->p+__pel->off;
	if (src>=__pel->end) {
		ffly_printf("pellet error, get\n");
		return;
	}
	ffly_mem_cpy(__p, src, __size);
}

void ffly_pellet_incr(ffly_pelletp __pel, mdl_uint_t __by) {
	if (__pel->p+(__pel->off+__by)>__pel->end) {
		ffly_printf("pellet error, incr\n");
		return;
	}
	__pel->off+=__by;
}

void ffly_pellet_decr(ffly_pelletp __pel, mdl_uint_t __by) {
	if (__by>__pel->off) {
		ffly_printf("pellet error, decr{%u>%u}\n", __by, __pel->off);
		return;
	}
	__pel->off-=__by;
}

void ffly_pellet_free(ffly_pelletp __pel) {
	__ffly_mem_free(__pel->p);
	__ffly_mem_free(__pel);
}
