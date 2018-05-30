# ifndef __ffly__linux__types__h
# define __ffly__linux__types__h
# include "../ffint.h"

// remove linux so __pid_t
typedef ff_s32_t __linux_pid_t;
typedef ff_s64_t __linux_off_t;
typedef ff_s64_t __linux_loff_t;

typedef ff_s32_t __linux_key_t;
typedef ff_s32_t __linux_mqd_t;

typedef ff_u32_t __linux_uid_t;
typedef ff_u32_t __linux_gid_t;
typedef ff_u32_t __linux_mode_t;
typedef ff_s64_t __linux_time_t;
typedef ff_s32_t __linux_timer_t;

typedef ff_s32_t __linux_ipc_pid_t;
typedef ff_u64_t __linux_size_t;
typedef ff_s64_t __linux_ssize_t;

typedef ff_s64_t __linux_suseconds_t;
typedef ff_u16_t __linux_sa_family_t;
typedef ff_s32_t __linux_clockid_t;
typedef	ff_u64_t __linux_clock_t;
typedef ff_u16_t __linux_umode_t;
# endif /*__ffly__linux__type__h*/
