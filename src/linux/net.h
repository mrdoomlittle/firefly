# ifndef __ffly__linux__net__h
# define __ffly__linux__net__h

enum __linux_sock_type {
	SOCK_STREAM	= 1,
	SOCK_DGRAM	= 2,
	SOCK_RAW	= 3,
	SOCK_RDM	= 4,
	SOCK_SEQPACKET	= 5,
	SOCK_DCCP	= 6,
	SOCK_PACKET	= 10,
};

# endif /*__ffly__linux__net__h*/
