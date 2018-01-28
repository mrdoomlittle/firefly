# ifndef __ffly__net__h
# define __ffly__net__h
# include "types/err_t.h"
# include "network/header.h"
# include "network/sock.h"
# include <mdlint.h>
# define FF_MTU 20000
FF_SOCKET* ff_net_creat(ffly_err_t*, int, int, int);
ffly_err_t ff_net_connect(FF_SOCKET*, struct sockaddr*, socklen_t);
FF_SOCKET* ff_net_accept(FF_SOCKET*, struct sockaddr*, socklen_t*, ffly_err_t*);
ffly_size_t ff_net_send(FF_SOCKET*, void*, mdl_uint_t, ffly_err_t*);
ffly_size_t ff_net_recv(FF_SOCKET*, void*, mdl_uint_t, ffly_err_t*);
ffly_err_t ff_net_bind(FF_SOCKET*, struct sockaddr*, socklen_t);
ffly_err_t ff_net_listen(FF_SOCKET*);
ffly_err_t ff_net_close(FF_SOCKET*);
ffly_err_t ff_net_sndhdr(FF_SOCKET*, FF_NET_HDR*);
ffly_err_t ff_net_rcvhdr(FF_SOCKET*, FF_NET_HDR*);
# endif /*__ffly__net__h*/
