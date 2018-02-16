# ifndef __ffly__linux__stat__h
# define __ffly__linux__stat__h
# include <mdlint.h>
# include "time.h"
# ifndef S_IRWXU
# define S_IRWXU 00700
# endif
# ifndef S_IRUSR
# define S_IRUSR 00400
# endif
# ifndef S_IWUSR
# define S_IWUSR 00200
# endif
# ifndef S_IXUSR
# define S_IXUSR 00100
# endif
# ifndef S_IRWXG
# define S_IRWXG 00070
# endif 
# ifndef S_IRGRP
# define S_IRGRP 00040
# endif
# ifndef S_IWGRP
# define S_IWGRP 00020
# endif
# ifndef S_IXGRP
# define S_IXGRP 00010
# endif
# ifndef S_IRWXO
# define S_IRWXO 00007
# endif
# ifndef S_IROTH
# define S_IROTH 00004
# endif
# ifndef S_IWOTH
# define S_IWOTH 00002
# endif
# ifndef S_IXOTH
# define S_IXOTH 00001
# endif
struct stat { //needs testing
	unsigned long st_dev;
	unsigned long st_ino;
	unsigned long st_nlink;
	unsigned int st_mode;
	unsigned int st_uid;
	unsigned int st_gid;
	int __pad0;
	unsigned long st_rdev;
	long st_size;
	int long st_blksize;	
	int __pad1;
	long st_blocks;
	__linux_time_t st_atime;
	mdl_u64_t st_atime_nsec;
	__linux_time_t st_mtime;	
	mdl_u64_t st_mtime_nsec;
	__linux_time_t st_ctime;
	mdl_u64_t st_ctime_nsec;
	unsigned long __unused4;
	unsigned long __unused5;
};

mdl_s32_t stat(char const*, struct stat*);
mdl_s32_t fstat(mdl_s32_t, struct stat*);
# endif /*__ffly__linux__stat__h*/
