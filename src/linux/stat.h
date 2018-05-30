# ifndef __ffly__linux__stat__h
# define __ffly__linux__stat__h
# include "../ffint.h"
# include "time.h"

#define S_IFMT 00170000
#define S_IFSOCK 0140000
#define S_IFLNK 0120000
#define S_IFREG 0100000
#define S_IFBLK 0060000
#define S_IFDIR 0040000
#define S_IFCHR 0020000
#define S_IFIFO 0010000
#define S_ISUID 0004000
#define S_ISGID 0002000
#define S_ISVTX 0001000

#define S_ISLNK(__m)  (((__m)&S_IFMT) == S_IFLNK)
#define S_ISREG(__m)  (((__m)&S_IFMT) == S_IFREG)
#define S_ISDIR(__m)  (((__m)&S_IFMT) == S_IFDIR)
#define S_ISCHR(__m)  (((__m)&S_IFMT) == S_IFCHR)
#define S_ISBLK(__m)  (((__m)&S_IFMT) == S_IFBLK)
#define S_ISFIFO(__m) (((__m)&S_IFMT) == S_IFIFO)
#define S_ISSOCK(__m) (((__m)&S_IFMT) == S_IFSOCK)

# define S_IRWXU 00700 // user: r/w/x
# define S_IRUSR 00400 // user: r
# define S_IWUSR 00200 // user: w
# define S_IXUSR 00100 // user: x

# define S_IRWXG 00070 // group: r/w/x
# define S_IRGRP 00040 // group: r
# define S_IWGRP 00020 // group: w
# define S_IXGRP 00010 // group: x

# define S_IRWXO 00007 // others: r/w/x
# define S_IROTH 00004 // others: r
# define S_IWOTH 00002 // others: w
# define S_IXOTH 00001 // others: x
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

ff_s32_t mkdir(char const*, __linux_mode_t);
ff_s32_t rmdir(char const*);
# endif /*__ffly__linux__stat__h*/
