# ifndef __smem__buff__h
# define __smem__buff__h
# include <eint_t.h>
# include "errno.h"
# include <errno.h>
# include "../types/err_t.h"
# include "../memory/mem_alloc.h"
# include "../memory/mem_free.h"
# include "../memory/mem_realloc.h"
# ifdef __cplusplus
extern "C"{
# endif

struct ffly_smem_buff_t {
	mdl_u8_t* dbuff;
	mdl_uint_t blk_len;
	mdl_uint_t buff_size;
	mdl_uint_t nxt_point;
};

struct ffly_smem_buff_t* ffly_smem_buff_ninst(mdl_uint_t, mdl_uint_t);
ffly_err_t ffly_smem_buff_init(struct ffly_smem_buff_t*, mdl_uint_t, mdl_uint_t);
ffly_err_t ffly_smem_buff_push(struct ffly_smem_buff_t*, void*);
ffly_err_t ffly_smem_buff_pop(struct ffly_smem_buff_t*, void*);
ffly_err_t ffly_smem_buff_ptr(struct ffly_smem_buff_t*, void**);
mdl_u8_t __inline__ ffly_smem_buff_full(struct ffly_smem_buff_t *__smem_buff) {
	return __smem_buff-> nxt_point == __smem_buff-> buff_size - 1? 1 : 0;}

mdl_u8_t __inline__ ffly_smem_buff_empty(struct ffly_smem_buff_t *__smem_buff) {
	return !__smem_buff-> nxt_point? 1 : 0;}

ffly_err_t __inline__ ffly_smem_buff_de_init(struct ffly_smem_buff_t *__smem_buff) {
	__ffly_mem_free(__smem_buff-> dbuff);
}

# ifdef __cplusplus
}
# endif
# endif /*__smem__buff__h*/
