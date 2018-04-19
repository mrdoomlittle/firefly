# ifndef __ffly__linux__wait__h
# define __ffly__linux__wait__h
# include "types.h"
# include "resource.h"
#define WNOHANG		0x00000001
#define WUNTRACED	0x00000002
#define WSTOPPED	WUNTRACED
#define WEXITED		0x00000004
#define WCONTINUED	0x00000008
#define WNOWAIT		0x01000000

#define __WNOTHREAD 0x20000000
#define __WALL		0x40000000
#define __WCLONE	0x80000000
__linux_pid_t wait4(__linux_pid_t, int*, int, struct rusage*);
# endif /*__ffly__linux__wait__h*/
