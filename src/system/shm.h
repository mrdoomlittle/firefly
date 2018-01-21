# ifndef __ffly__shm__h
# define __ffly__shm__h
# include <mdlint.h>
# define FF_SHM_MCI 0x1
void* ffly_shmget(mdl_uint_t*, mdl_uint_t, int, mdl_u8_t);
void ffly_shmdt(mdl_uint_t);
void ffly_shm_free(mdl_uint_t);
void ffly_shm_cleanup(mdl_uint_t);
# endif /*__ffly__shm__h*/
