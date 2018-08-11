# include "m.h"
# include "memory/mem_alloc.h"
# include "memory/mem_free.h"
# include "memory/mem_realloc.h"
# include "system/io.h"

ff_u64_t ffly_mem_usage(void) {
# ifdef __ffly_debug
	return ffly_mem_alloc_bc-ffly_mem_free_bc;
# else
	return 0;
# endif	
}

void ffly_meminfo(struct ffly_meminfo *__info) {
# ifdef __ffly_debug
	__info->used = ffly_mem_alloc_bc-ffly_mem_free_bc;
	__info->ac = ffly_mem_alloc_c;
	__info->fc = ffly_mem_free_c;
# endif
}

void ffly_dmi(struct ffly_meminfo *__info) {
# ifdef __ffly_debug
	ffly_printf("memused: %u.%u-kilobytes.\n", __info->used>>10, __info->used&0x1023);
	ffly_printf("allocation count: %u.\n", __info->ac);
	ffly_printf("free count: %u.\n", __info->fc);
# endif
}
