# ifndef __ffly__linux__stat__h
# define __ffly__linux__stat__h
# include "../ffint.h"
# include "time.h"
# ifndef S_IRWXU
# define S_IRWXU 00700 // user: r/w/x
# endif
# ifndef S_IRUSR
# define S_IRUSR 00400 // user: r
# endif
# ifndef S_IWUSR
# define S_IWUSR 00200 // user: w
# endif
# ifndef S_IXUSR
# define S_IXUSR 00100 // user: x
# endif
# ifndef S_IRWXG
# define S_IRWXG 00070 // group: r/w/x
# endif 
# ifndef S_IRGRP
# define S_IRGRP 00040 // group: r
# endif
# ifndef S_IWGRP
# define S_IWGRP 00020 // group: w
# endif
# ifndef S_IXGRP
# define S_IXGRP 00010 // group: x
# endif
# ifndef S_IRWXO
# define S_IRWXO 00007 // others: r/w/x
# endif
# ifndef S_IROTH
# define S_IROTH 00004 // others: r
# endif
# ifndef S_IWOTH
# define S_IWOTH 00002 // others: w
# endif
# ifndef S_IXOTH
# define S_IXOTH 00001 // others: x
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
	ff_u64_t st_atime_nsec;
	__linux_time_t st_mtime;	
	ff_u64_t st_mtime_nsec;
	__linux_time_t st_ctime;
	ff_u64_t st_ctime_nsec;
	unsigned long __unused4;
	unsigned long __unused5;
};

ff_s32_t stat(char const*, struct stat*);
ff_s32_t fstat(ff_s32_t, struct stat*);
# endif /*__ffly__linux__stat__h*/
