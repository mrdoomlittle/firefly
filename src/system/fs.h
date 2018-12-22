# ifndef __ffly__system__fs__h
# define __ffly__system__fs__h
# include "dir.h"
# include "file.h"

#define FF_POSIX_FS 0

struct ffly_fsop {
	ff_s32_t(*open)(char const*, ff_u32_t, ff_u32_t);
	ff_s32_t(*close)(ff_u32_t);
	ff_s32_t(*lseek)(ff_u32_t, ff_u64_t, ff_u32_t);
	ff_s32_t(*read)(ff_u32_t, void*, ff_u64_t);
	ff_s32_t(*write)(ff_u32_t, void*, ff_u64_t);
	ff_s32_t(*pwrite)(ff_u32_t, void*, ff_u64_t, ff_u64_t);
	ff_s32_t(*pread)(ff_u32_t, void*, ff_u64_t, ff_u64_t);
	ff_s32_t(*access)(char const*, ff_u32_t);
	ff_s32_t(*creat)(char const*, ff_u32_t);
};

#define sfs(__func, ...)\
	sysfs->__func(__VA_ARGS__)
#define fsopen(...)\
	sfs(open, __VA_ARGS__)
#define fsclose(...)\
    sfs(close, __VA_ARGS__)
#define fslseek(...)\
    sfs(lseek, __VA_ARGS__)
#define fsread(...)\
    sfs(read, __VA_ARGS__)
#define fswrite(...)\
    sfs(write, __VA_ARGS__)
#define fspread(...)\
    sfs(pread, __VA_ARGS__)
#define fspwrite(...)\
    sfs(pwrite, __VA_ARGS__)
#define fsaccess(...)\
    sfs(access, __VA_ARGS__)
#define fscreat(...)\
    sfs(creat, __VA_ARGS__)
extern struct ffly_fsop *sysfs;
void ffly_sysfs(ff_u8_t);
# endif /*__ffly__system__fs__h*/
