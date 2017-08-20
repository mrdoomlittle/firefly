# include "mem_dupe.h"
# include "mem_ncpy.h"
# include "../memory/mem_alloc.h"
# include "../system/io.h"
# include "../system/errno.h"
ffly_err_t ffly_mem_dupe(void **__dst, void *__src, mdl_uint_t __bc) {
	if ((*__dst = __ffly_mem_alloc(__bc)) == NULL) {
		ffly_printf(stderr, "mem_dupe: failed to alloc memory, errno: %d, ffly_errno: %d.\n", errno, ffly_errno);
		return FFLY_FAILURE;
	}
	ffly_mem_ncpy(*__dst, __src, __bc);
	return FFLY_SUCCESS;
}
