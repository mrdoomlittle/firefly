# ifndef __ffly__net__h
# define __ffly__net__h
# include "ffint.h"
# include "types.h"
# include "network/header.h"
# include "network/sock.h"
# define FF_MTU 20000
// add udp

ff_err_t ff_net_shutdown(FF_SOCKET*, int);
FF_SOCKET* ff_net_creat(ff_err_t*, int, int, int);
ff_err_t ff_net_connect(FF_SOCKET*, struct sockaddr*, socklen_t);
FF_SOCKET* ff_net_accept(FF_SOCKET*, struct sockaddr*, socklen_t*, ff_err_t*);
ff_size_t ff_net_send(FF_SOCKET*, void const*, ff_uint_t, ff_err_t*);
ff_size_t ff_net_recv(FF_SOCKET*, void*, ff_uint_t, ff_err_t*);
ff_size_t ff_net_sendto(FF_SOCKET*, struct sockaddr*, socklen_t, void const*, ff_uint_t, ff_err_t*);
ff_size_t ff_net_recvfrom(FF_SOCKET*, struct sockaddr*, socklen_t*, void*, ff_uint_t, ff_err_t*);

ff_err_t ff_net_bind(FF_SOCKET*, struct sockaddr*, socklen_t);
ff_err_t ff_net_listen(FF_SOCKET*);
ff_err_t ff_net_close(FF_SOCKET*);
ff_err_t ff_net_sndhdr(FF_SOCKET*, FF_NET_HDR*);
ff_err_t ff_net_rcvhdr(FF_SOCKET*, FF_NET_HDR*);
# endif /*__ffly__net__h*/
