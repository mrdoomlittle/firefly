# include "mem_allocr.h"

ffly_err_t ffly_mem_allocr_init(struct ffly_mem_allocr_t *__mem_allocr) {
	if ((__mem_allocr-> mem_stack = ffly_mem_alloc(__mem_allocr-> stack_size, 0)) == NULL) {
		fprintf(stderr, "mem_allocr: failed to alloc mem stack, errno: %d\n", errno);
		return FFLY_FAILURE;
	}
	__mem_allocr-> nxt_addr = __mem_allocr-> mem_stack;
	__mem_allocr-> mem_free = __mem_allocr-> stack_size;
	__mem_allocr-> mem_used = 0;
	return FFLY_SUCCESS;
}

ffly_err_t ffly_mem_allocr_de_init(struct ffly_mem_allocr_t *__mem_allocr) {
	ffly_mem_free(__mem_allocr-> mem_stack);
}

ffly_err_t ffly_mem_allocr_alloc(struct ffly_mem_allocr_t *__mem_allocr, void **__mptr, mdl_uint_t __bc) {
	if (__mem_allocr-> mem_free < __bc) {
		fprintf(stderr, "mem_allocr: no memory left on stack.\n");
		return FFLY_FAILURE;
	}

	*(mdl_uint_t *)__mem_allocr-> nxt_addr = __bc;
	*__mptr = __mem_allocr-> nxt_addr + sizeof(mdl_uint_t);
	__mem_allocr-> nxt_addr += __bc;
	__mem_allocr-> mem_free -= __bc;
	__mem_allocr-> mem_used += __bc;
	return FFLY_SUCCESS;
}



ffly_err_t ffly_mem_allocr_free(struct ffly_mem_allocr_t *__mem_allocr, void *__mptr) {
	mdl_uint_t bc = *(mdl_uint_t *)__mptr;

	__mem_allocr-> mem_free += bc;
	__mem_allocr-> mem_used -= bc;
	return FFLY_SUCCESS;
}
