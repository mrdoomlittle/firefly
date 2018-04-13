# ifndef __ffly__linux__socket__h
# define __ffly__linux__socket__h
# include "../ffint.h"
# include "types.h"
# include "../types/socket.h"

struct sockaddr {
	__linux_sa_family_t sa_family;
	char sa_data[14];
};

#define SOL_SOCKET	1

#define SO_DEBUG		1
#define SO_REUSEADDR    2
#define SO_TYPE			3
#define SO_ERROR		4
#define SO_DONTROUTE	5
#define SO_BROADCAST	6
#define SO_SNDBUF		7
#define SO_RCVBUF		8
#define SO_SNDBUFFORCE	32
#define SO_RCVBUFFORCE	33
#define SO_KEEPALIVE	9
#define SO_OOBINLINE    10
#define SO_NO_CHECK		11
#define SO_PRIORITY		12
#define SO_LINGER		13
#define SO_BSDCOMPAT	14
#define SO_REUSEPORT	15

#define AF_UNSPEC	0
#define AF_UNIX		1
#define AF_LOCAL	1
#define AF_INET		2
#define AF_AX25		3
#define AF_IPX		4
#define AF_APPLETALK	5
#define AF_NETROM	6
#define AF_BRIDGE	7
#define AF_ATMPVC	8
#define AF_X25		9
#define AF_INET6	10
#define AF_ROSE		11
#define AF_DECnet	12
#define AF_NETBEUI	13
#define AF_SECURITY	14
#define AF_KEY		15
#define AF_NETLINK	16
#define AF_ROUTE	AF_NETLINK
#define AF_PACKET	17
#define AF_ASH		18
#define AF_ECONET	19
#define AF_ATMSVC	20
#define AF_RDS		21
#define AF_SNA		22
#define AF_IRDA		23
#define AF_PPPOX	24
#define AF_WANPIPE	25
#define AF_LLC		26
#define AF_IB		27
#define AF_MPLS		28
#define AF_CAN		29
#define AF_TIPC		30
#define AF_BLUETOOTH	31
#define AF_IUCV		32
#define AF_RXRPC	33
#define AF_ISDN		34
#define AF_PHONET	35
#define AF_IEEE802154	36
#define AF_CAIF		37
#define AF_ALG		38
#define AF_NFC		39
#define AF_VSOCK	40
#define AF_KCM		41
#define AF_QIPCRTR	42
#define AF_SMC		43

#define AF_MAX		44

#define PF_UNSPEC	AF_UNSPEC
#define PF_UNIX		AF_UNIX
#define PF_LOCAL	AF_LOCAL
#define PF_INET		AF_INET
#define PF_AX25		AF_AX25
#define PF_IPX		AF_IPX
#define PF_APPLETALK	AF_APPLETALK
#define	PF_NETROM	AF_NETROM
#define PF_BRIDGE	AF_BRIDGE
#define PF_ATMPVC	AF_ATMPVC
#define PF_X25		AF_X25
#define PF_INET6	AF_INET6
#define PF_ROSE		AF_ROSE
#define PF_DECnet	AF_DECnet
#define PF_NETBEUI	AF_NETBEUI
#define PF_SECURITY	AF_SECURITY
#define PF_KEY		AF_KEY
#define PF_NETLINK	AF_NETLINK
#define PF_ROUTE	AF_ROUTE
#define PF_PACKET	AF_PACKET
#define PF_ASH		AF_ASH
#define PF_ECONET	AF_ECONET
#define PF_ATMSVC	AF_ATMSVC
#define PF_RDS		AF_RDS
#define PF_SNA		AF_SNA
#define PF_IRDA		AF_IRDA
#define PF_PPPOX	AF_PPPOX
#define PF_WANPIPE	AF_WANPIPE
#define PF_LLC		AF_LLC
#define PF_IB		AF_IB
#define PF_MPLS		AF_MPLS
#define PF_CAN		AF_CAN
#define PF_TIPC		AF_TIPC
#define PF_BLUETOOTH	AF_BLUETOOTH
#define PF_IUCV		AF_IUCV
#define PF_RXRPC	AF_RXRPC
#define PF_ISDN		AF_ISDN
#define PF_PHONET	AF_PHONET
#define PF_IEEE802154	AF_IEEE802154
#define PF_CAIF		AF_CAIF
#define PF_ALG		AF_ALG
#define PF_NFC		AF_NFC
#define PF_VSOCK	AF_VSOCK
#define PF_KCM		AF_KCM
#define PF_QIPCRTR	AF_QIPCRTR
#define PF_SMC		AF_SMC
#define PF_MAX		AF_MAX

#define SOL_IP		0
#define SOL_TCP		6
#define SOL_UDP		17
#define SOL_IPV6	41
#define SOL_ICMPV6	58
#define SOL_SCTP	132
#define SOL_UDPLITE	136
#define SOL_RAW		255
#define SOL_IPX		256
#define SOL_AX25	257
#define SOL_ATALK	258
#define SOL_NETROM	259
#define SOL_ROSE	260
#define SOL_DECNET	261
#define	SOL_X25		262
#define SOL_PACKET	263
#define SOL_ATM		264
#define SOL_AAL		265
#define SOL_IRDA        266
#define SOL_NETBEUI	267
#define SOL_LLC		268
#define SOL_DCCP	269
#define SOL_NETLINK	270
#define SOL_TIPC	271
#define SOL_RXRPC	272
#define SOL_PPPOL2TP	273
#define SOL_BLUETOOTH	274
#define SOL_PNPIPE	275
#define SOL_RDS		276
#define SOL_IUCV	277
#define SOL_CAIF	278
#define SOL_ALG		279
#define SOL_NFC		280
#define SOL_KCM		281
#define SOL_TLS		282


#define SHUT_RD 0
#define SHUT_WR 1
#define SHUT_RDWR 2
ff_s32_t shutdown(ff_u32_t, ff_u32_t);
ff_s32_t connect(ff_u32_t, struct sockaddr*, sockl_t);
ff_s32_t socket(ff_u32_t, ff_u32_t, ff_u32_t);
ff_s32_t bind(ff_u32_t, struct sockaddr*, sockl_t);
ff_s32_t accept(ff_u32_t, struct sockaddr*, sockl_t*);
ff_s32_t listen(ff_u32_t, ff_u32_t);
ff_s32_t sendto(ff_u32_t, void*, ff_u32_t, ff_u32_t, struct sockaddr*, sockl_t);
ff_s32_t recvfrom(ff_u32_t, void*, ff_u32_t, ff_u32_t, struct sockaddr*, sockl_t*);
ff_s32_t send(ff_u32_t, void*, ff_u32_t, ff_u32_t);
ff_s32_t recv(ff_u32_t, void*, ff_u32_t, ff_u32_t);
ff_s32_t setsockopt(ff_u32_t, ff_u32_t, ff_u32_t, void*, ff_u32_t);
ff_s32_t getsockopt(ff_u32_t, ff_u32_t, ff_u32_t, void*, ff_u32_t*);
# endif /*__ffly__linux__socket__h*/
