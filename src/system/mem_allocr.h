# ifndef __ffly__mem__allocr__h
# define __ffly__mem__allocr__h
# include <eint_t.h>
# include "../types/err_t.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "errno.h"
# include <errno.h>
# include <stdio.h>

struct ffly_mem_allocr_t {
	mdl_u8_t *mem_stack, *nxt_addr;
	mdl_uint_t stack_size;
	mdl_uint_t mem_free, mem_used;
};

ffly_err_t ffly_mem_allocr_init(struct ffly_mem_allocr_t *);
ffly_err_t ffly_mem_allocr_de_init(struct ffly_mem_allocr_t *);

ffly_err_t ffly_mem_allocr_alloc(struct ffly_mem_allocr_t *, void **, mdl_uint_t __bc);
ffly_err_t ffly_mem_allocr_free(struct ffly_mem_allocr_t *, void *);
ffly_err_t ffly_mem_allocr_realloc(struct ffly_mem_allocr_t *, void **, mdl_uint_t __bc);

# endif /*__ffly__mem__allocr__h*/
