# ifndef __ffly__net__h
# define __ffly__net__h
# include "network/peer.h"
# include "types/err_t.h"
# include <mdlint.h>
ffly_net_peerp ff_net_creat(ffly_err_t*);
ffly_err_t ff_net_connect(ffly_net_peerp);
ffly_err_t ff_net_accept(ffly_net_peerp);
ffly_err_t ff_net_send(ffly_net_peerp, void*, mdl_uint_t);
ffly_err_t ff_net_recv(ffly_net_peerp, void*, mdl_uint_t);
ffly_err_t ff_net_close(ffly_net_peerp);
# endif /*__ffly__net__h*/
