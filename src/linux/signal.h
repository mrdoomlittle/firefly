# ifndef __ffly__linux__signal__h
# define __ffly__linux__signal__h
# include "../ffint.h"
# include "types.h"

typedef void(*__sighandler_t)(int);
typedef void(*__sigrestore_t)(void);


typedef unsigned long sigset_t;

typedef union sigval  {
	int sival_int;
	void *sival_ptr;
} sigval_t;


#define SIGEV_SIGNAL	0
#define SIGEV_NONE	1 
#define SIGEV_THREAD	2
#define SIGEV_THREAD_ID	4

#define SIGEV_MAX_SIZE 64
struct sigevent {
	sigval_t sigev_value;
	int sigev_signo;
	int sigev_notify;
	union {
#if defined(__ff64) || !defined(__ff32)
		int _pad[(SIGEV_MAX_SIZE/sizeof(int))-4];
#else
		int _pad[(SIGEV_MAX_SIZE/sizeof(int))-3];
#endif
		int _tid;
		struct {
			void(*_function)(sigval_t);
			void *_attribute;
		} _sigev_thread;
	} _sigev_un;
};

#define SI_MAX_SIZE 128
// not tested
typedef struct siginfo {
	int si_signo;
	int si_errno;
	int si_code;
	union {
#if defined(__ff64) || !defined(__ff32)
	int _pad[(SI_MAX_SIZE/sizeof(int))-4];
# else
	int _pad[(SI_MAX_SIZE/sizeof(int))-3];
# endif
	struct {
		__linux_pid_t _pid;
		__linux_uid32_t _uid;
	} _kill;

	struct {
		__linux_timer_t _tid;
		int _overrun;
		sigval_t _sigval;
		int _sys_private;
	} _timer;

	struct {
		__linux_pid_t _pid;
		__linux_uid32_t _uid;
		sigval_t _sigval;
	} _rt;

	struct {
		__linux_pid_t _pid;
		__linux_uid32_t _uid;
		int _status;
		__linux_clock_t _utime;
		__linux_clock_t _stime;
	} _sigchld;

	struct {
		void *_addr;
		union {
			short _addr_lsb;
			struct {
				void *_lower;
				void *_upper;
			} _addr_bnd;
			ff_u32_t _pkey;
		};
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
#define si_value _sifields._rt._sigval
} siginfo_t;

struct sigaction {	
	__sighandler_t sa_handler;
	unsigned long sa_flags;
	__sigrestore_t sa_restorer;
	sigset_t sa_mask;
#define sa_sigaction sa_handler
};

ff_s32_t kill(__linux_pid_t, ff_s32_t);
ff_s32_t rt_sigaction(ff_s32_t, struct sigaction const*, struct sigaction*, ff_u32_t);
#define SIGHUP		1
#define SIGINT		2
#define SIGQUIT		3
#define SIGILL		4
#define SIGTRAP		5
#define SIGABRT 	6
#define SIGIOT		6
#define SIGBUS		7
#define SIGFPE		8
#define SIGKILL		9
#define SIGUSR1		10
#define SIGSEGV		11
#define SIGUSR2		12
#define SIGPIPE		13
#define SIGALRM		14
#define SIGTERM		15
#define SIGSTKFLT	16
#define SIGCHLD		17
#define SIGCONT		18
#define SIGSTOP		19
#define SIGTSTP		20
#define SIGTTIN		21
#define SIGTTOU		22
#define SIGURG		23
#define SIGXCPU		24
#define SIGXFSZ		25
#define SIGVTALRM	26
#define SIGPROF		27
#define SIGWINCH	28
#define SIGIO		29
#define SIGPOLL		SIGIO
#define SIGPWR		30
#define SIGSYS		31
#define SIGUNUSED	31
#define SIGRTMIN	32

#define SA_NOCLDSTOP	0x00000001u
#define SA_NOCLDWAIT	0x00000002u
#define SA_SIGINFO		0x00000004u
#define SA_ONSTACK		0x08000000u
#define SA_RESTART		0x10000000u
#define SA_NODEFER		0x40000000u
#define SA_RESETHAND	0x80000000u

#define SA_NOMASK	SA_NODEFER
#define SA_ONESHOT	SA_RESETHAND

#define SA_RESTORER	0x04000000
# endif /*__ffly__linux__signal__h*/
