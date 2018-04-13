# ifndef __ffly__linux__in__h
# define __ffly__linux__in__h
# include "../ffint.h"
# include "types.h"
struct in_addr {
	ff_u32_t s_addr;
};

#define __SOCK_SIZE__ 16
struct sockaddr_in {
	__linux_sa_family_t sin_family;
	ff_u16_t sin_port;
	struct in_addr sin_addr;
	ff_u8_t __pad[__SOCK_SIZE__-sizeof(ff_u16_t)-
		sizeof(__linux_sa_family_t)-sizeof(struct in_addr)];
};

# endif /*__ffly__linux__in__h*/
