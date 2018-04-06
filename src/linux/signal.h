# ifndef __ffly__linux__signal__h
# define __ffly__linux__signal__h
# include <mdlint.h>
# include "types.h"

typedef void(*__sighandler_t)(int);
typedef void(*__sigrestore_t)(void);


typedef unsigned long sigset_t;

#define SI_MAX_SIZE	128

typedef union sigval  {
	int sival_int;
	void *sival_ptr;
} sigval_t;

// not tested
typedef struct siginfo {
	int si_signo;
	int si_errno;
	int si_code;
	union {
# if __WORDSIZE == 64
	int __pad[(SI_MAX_SIZE-4)/sizeof(int)];
# else
	int __pad[(SI_MAX_SIZE-3)/sizeof(int)];
# endif
	struct {
		__linux_pid_t _pid;
		__linux_uid_t _uid;
	} _kill;

	struct {
		__linux_timer_t _tid;
		int _overrun;
		sigval_t _sigval;
		int _sys_private;
	} _timer;

	struct {
		__linux_pid_t _pid;
		__linux_uid_t _uid;
		sigval_t _sigval;
	} _rt;

	struct {
		__linux_pid_t _pid;
		__linux_uid_t _uid;
		int _status;
		__linux_clock_t _utime;
		__linux_clock_t _stime;
	} _sigchld;

	struct {
		void *_addr;
		short _addr_lsb;
		struct {
			void *_lower;
			void *_upper;
		} _addr_bnd;
	} _sigfault;

	struct {
		long _band;
		int _fd;
	} _sigpoll;

	struct {
		void *_call_addr;
		int _syscall;
		unsigned int _arch;
	} _sigsys;
	} _sifields;
} siginfo_t;

struct sigaction {	
	__sighandler_t sa_handler;
	unsigned long sa_flags;
	__sigrestore_t sa_restorer;
	sigset_t sa_mask;
};

mdl_s32_t kill(__linux_pid_t, mdl_s32_t);
mdl_s32_t rt_sigaction(mdl_s32_t, struct sigaction const*, struct sigaction*, mdl_u32_t);
#define SIGKILL	9
#define SIGINT	2
#define SIGCHLD	17

#define SA_RESTORER	0x04000000
# endif /*__ffly__linux__signal__h*/
