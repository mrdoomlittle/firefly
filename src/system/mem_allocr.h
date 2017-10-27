# ifndef __ffly__mem__allocr__h
# define __ffly__mem__allocr__h
# include <mdlint.h>
# include "../types/err_t.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# include "errno.h"
# include <errno.h>
# include <stdio.h>
# include <string.h>
# ifdef __cplusplus
extern "C" {
# endif

struct ffly_mem_allocr {
	mdl_uint_t stack_fs;
	mdl_uint_t alloc_c;
	mdl_u8_t *mem_stack, *next_addr;
	mdl_uint_t stack_size;
	mdl_uint_t mem_free, mem_used;
	void *last_blk;
	void *uu_blks;
};

ffly_err_t ffly_mem_allocr_init(struct ffly_mem_allocr*, mdl_uint_t);
ffly_err_t ffly_mem_allocr_de_init(struct ffly_mem_allocr*);
mdl_uint_t __inline__ ffly_mem_allocr_mem_used(struct ffly_mem_allocr *__mem_allocr) {
	return __mem_allocr-> mem_used;}

mdl_uint_t __inline__ ffly_mem_allocr_mem_free(struct ffly_mem_allocr *__mem_allocr) {
	return __mem_allocr-> mem_free;}
ffly_err_t ffly_mem_allocr_alloc(struct ffly_mem_allocr*, void**, mdl_uint_t __bc);
ffly_err_t ffly_mem_allocr_free(struct ffly_mem_allocr*, void*);
ffly_err_t ffly_mem_allocr_realloc(struct ffly_mem_allocr*, void**, mdl_uint_t __bc);
# ifdef __cplusplus
}
# endif
# endif /*__ffly__mem__allocr__h*/
