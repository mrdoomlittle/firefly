# ifndef __ffly__linux__unistd__h
# define __ffly__linux__unistd__h
# include "types.h"

# define R_OK 4
# define W_OK 2
# define X_OK 1
# define F_OK 0
__linux_pid_t ffly_getpid(void);
__linux_pid_t ffly_getppid(void);

mdl_s32_t ffly_read(mdl_u32_t, void*, mdl_u64_t);
mdl_s32_t ffly_write(mdl_u32_t, void*, mdl_u64_t);
mdl_s32_t ffly_access(char const*, mdl_u32_t);

mdl_s32_t ffly_fsync(mdl_s32_t);
mdl_s32_t ffly_lseek(mdl_u32_t, mdl_u64_t, mdl_u32_t);
void ffly_exit(mdl_s32_t);
# endif /*__ffly__linux__unistd__h*/
