# ifndef __ffly__linux__ipc__h
# define __ffly__linux__ipc__h
# include "types.h"

# define IPC_CREAT 00001000

# define IPC_RMID 0
# define IPC_SET 1
# define IPC_STAT 2
# define IPC_INFO 3

struct ipc_perm {
	__linux_key_t key;
	__linux_uid_t uid;
	__linux_gid_t gid;
	__linux_gid_t cuid;
	__linux_uid_t cgid;
	__linux_gid_t mode;
	mdl_u16_t seq;
};

# endif /*__ffly__linux__ipc__h*/
