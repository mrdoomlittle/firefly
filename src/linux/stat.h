# ifndef __ffly__linux__stat__h
# define __ffly__linux__stat__h

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
struct __linux_stat {
	unsigned long st_dev;
	unsigned long st_ino;
	unsigned int st_mode;
	unsigned int st_nlink;
	unsigned int st_uid;
	unsigned int st_gid;
	unsigned long st_rdev;
	unsigned long __pad1;
	long st_size;
	int st_blksize;	
	int __pad2;
	long st_blocks;
	long st_atime;
	unsigned long st_atime_nsec;
	long st_mtime;	
	unsigned long st_mtime_nsec;
	long st_ctime;
	unsigned long st_ctime_nsec;
	unsigned int __unused4;
	unsigned int __unused5;
};

mdl_s32_t ffly_stat(char const*, struct __linux_stat*);
# endif /*__ffly__linux__stat__h*/
