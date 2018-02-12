# ifndef __ffly__linux__shm__h
# define __ffly__linux__shm__h
# include "../types.h"
# include "../ipc.h"
# include <mdlint.h>
struct shmid_ds {
	struct ipc_perm shm_perm;
	mdl_s32_t shm_segsz;
	__linux_time_t shm_atime;
	__linux_time_t shm_dtime;
	__linux_time_t shm_ctime;
	__linux_ipc_pid_t shm_cpid;
	__linux_ipc_pid_t shm_lpid;
	mdl_u16_t shm_nattch;
	mdl_u16_t shm_unused;
	void *shm_unused2;
	void *shm_unused3;
};

mdl_s32_t shmctl(mdl_s32_t, mdl_s32_t, struct shmid_ds*);
mdl_s32_t shmget(__linux_key_t, __linux_size_t, mdl_s32_t);
void* shmat(mdl_s32_t, void*, mdl_s32_t);
mdl_s32_t shmdt(void*);
# endif /*__ffly__linux__shm__h*/
