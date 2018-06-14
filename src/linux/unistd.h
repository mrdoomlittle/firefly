# ifndef __ffly__linux__unistd__h
# define __ffly__linux__unistd__h
# include "../ffint.h"
# include "types.h"

# define SEEK_SET 0
# define SEEK_CUR 1
# define SEEK_END 2

# define R_OK 4
# define W_OK 2
# define X_OK 1
# define F_OK 0
__linux_pid_t getpid(void);
__linux_pid_t getppid(void);

ff_s32_t open(char const*, ff_s32_t, ff_s32_t);

ff_s32_t read(ff_u32_t, void*, ff_u64_t);
ff_s32_t write(ff_u32_t, void*, ff_u64_t);
ff_s32_t pwrite(ff_u32_t, void*, ff_u64_t, ff_u64_t);
ff_s32_t pread(ff_u32_t, void*, ff_u64_t, ff_u64_t);
ff_s32_t access(char const*, ff_u32_t);

ff_s32_t fsync(ff_s32_t);
ff_s32_t lseek(ff_u32_t, ff_u64_t, ff_u32_t);
ff_s32_t exit(ff_s32_t);
ff_s32_t getcwd(char*, ff_u64_t);
void* brk(void*);

ff_s32_t execve(const char*, char *const*, char *const*);
ff_s32_t rmdir(char const*);
ff_s32_t unlink(char const*);
ff_s32_t ftruncate(ff_u32_t, ff_u32_t);
ff_s32_t dup2(ff_u32_t, ff_u32_t);
# endif /*__ffly__linux__unistd__h*/
