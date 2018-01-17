# ifndef __ffly__peer__h
# define __ffly__peer__h
# include <mdlint.h>
struct ffly_net_peer {
    mdl_u8_t proto; // tcp,udp?

};

typedef struct ffly_net_peer* ffly_net_peerp;
# endif /*__ffly__peer__h*/
