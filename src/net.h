# ifndef __ffly__net__h
# define __ffly__net__h
# include "ffint.h"
# include "types.h"
# include "network/header.h"
# include "network/sock.h"
#define FF_MTU_SHT 7
#define FF_MTU (1<<FF_MTU_SHT)
#define FF_STC (FF_MTU-sizeof(struct ff_net_sins))
// add udp

/*
	TODO:
		begin work on this

*/

enum {
	_NET_PROT_TCP,
	_NET_PROT_UDP
};

ff_err_t ff_net_shutdown(FF_SOCKET*, int);
FF_SOCKET* ff_net_creat(ff_err_t*, ff_u8_t);
ff_err_t ff_net_connect(FF_SOCKET*, struct sockaddr*, socklen_t);
FF_SOCKET* ff_net_accept(FF_SOCKET*, struct sockaddr*, socklen_t*, ff_err_t*);
ff_size_t ff_net_send(FF_SOCKET*, void const*, ff_uint_t, int, ff_err_t*);
ff_size_t ff_net_recv(FF_SOCKET*, void*, ff_uint_t, int, ff_err_t*);
ff_size_t ff_net_sendto(FF_SOCKET*, struct sockaddr*, socklen_t, void const*, ff_uint_t, int, ff_err_t*);
ff_size_t ff_net_recvfrom(FF_SOCKET*, struct sockaddr*, socklen_t*, void*, ff_uint_t, int, ff_err_t*);

ff_err_t ff_net_bind(FF_SOCKET*, struct sockaddr*, socklen_t);
ff_err_t ff_net_listen(FF_SOCKET*);
ff_err_t ff_net_close(FF_SOCKET*);
# endif /*__ffly__net__h*/
