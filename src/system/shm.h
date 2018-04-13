# ifndef __ffly__shm__h
# define __ffly__shm__h
# include "../ffint.h"
# include "../types.h"
# define FF_SHM_MCI 0x1
void* ffly_shmget(ff_uint_t*, ff_uint_t, int, ff_u8_t);
ff_err_t ffly_shmdt(ff_uint_t);
ff_err_t ffly_shm_free(ff_uint_t);
void ffly_shm_cleanup(ff_uint_t);
# endif /*__ffly__shm__h*/
