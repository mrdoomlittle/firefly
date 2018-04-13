# ifndef __ffly__linux__shm__h
# define __ffly__linux__shm__h
# include "../../ffint.h"
# include "../types.h"
# include "../ipc.h"

struct shmid_ds {
	struct ipc_perm shm_perm;
	ff_s32_t shm_segsz;
	__linux_time_t shm_atime;
	__linux_time_t shm_dtime;
	__linux_time_t shm_ctime;
	__linux_ipc_pid_t shm_cpid;
	__linux_ipc_pid_t shm_lpid;
	ff_u16_t shm_nattch;
	ff_u16_t shm_unused;
	void *shm_unused2;
	void *shm_unused3;
};

ff_s32_t shmctl(ff_s32_t, ff_s32_t, struct shmid_ds*);
ff_s32_t shmget(__linux_key_t, __linux_size_t, ff_s32_t);
void* shmat(ff_s32_t, void*, ff_s32_t);
ff_s32_t shmdt(void*);
# endif /*__ffly__linux__shm__h*/
