# ifndef __ffly__linux__unistd__h
# define __ffly__linux__unistd__h
# include "types.h"

# define R_OK 4
# define W_OK 2
# define X_OK 1
# define F_OK 0
__linux_pid_t getpid(void);
__linux_pid_t getppid(void);

mdl_s32_t open(char const*, mdl_s32_t, mdl_s32_t);

mdl_s32_t read(mdl_u32_t, void*, mdl_u64_t);
mdl_s32_t write(mdl_u32_t, void*, mdl_u64_t);
mdl_s32_t access(char const*, mdl_u32_t);

mdl_s32_t fsync(mdl_s32_t);
mdl_s32_t lseek(mdl_u32_t, mdl_u64_t, mdl_u32_t);
void exit(mdl_s32_t);
mdl_s32_t getcwd(char*, mdl_u64_t);
void* brk(void*);
# endif /*__ffly__linux__unistd__h*/
